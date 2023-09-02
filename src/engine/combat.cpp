//
// Created by Patryk Szczypień on 21/08/2023.
//
#include <combat.hpp>

void Combat::initialize() {
    _combat_dispatcher.sink<events::battle::AttackEvent>().connect<&Combat::attack>(this);
    register_attack_callback<damage_types::IceDmg, battle::AttackType::ICE>();
    register_attack_callback<damage_types::FireDmg, battle::AttackType::FIRE>();
    register_attack_callback<damage_types::PierceDmg, battle::AttackType::PIERCE>();
    register_damage_callback<damage_types::FireDmg>();
    register_damage_callback<damage_types::IceDmg>();
    register_damage_callback<damage_types::PierceDmg>();
    register_ailment_callback<damage_types::FireDmg, ailments::Burn>();
}

template<typename COMPONENT, battle::AttackType ATTACK_TYPE>
void Combat::register_attack_callback() {
    _core->registry.on_construct<COMPONENT>().template connect<&Combat::template register_damage<ATTACK_TYPE>>(this);
}

template<typename DAMAGE_TYPE>
void Combat::register_damage_callback (){
    _core->dispatcher.sink<events::battle::DamageEvent<DAMAGE_TYPE>>().template connect<&Combat::template on_damage_event<DAMAGE_TYPE>>(this);
    _core->dispatcher.sink<events::battle::FollowupEvent<DAMAGE_TYPE>>().template connect<&Combat::template followup_attack<DAMAGE_TYPE>>(this);
}

template<typename DAMAGE_TYPE, typename AILMENT>
void Combat::register_ailment_callback (){
    _core->dispatcher.sink<events::battle::AilmentEvent<DAMAGE_TYPE, AILMENT>>().template connect<&Combat::template cause_ailment<DAMAGE_TYPE, AILMENT>>(this);
}

template<typename DAMAGE_TYPE, typename AILMENT>
void Combat::cause_ailment(events::battle::AilmentEvent<DAMAGE_TYPE, AILMENT> event) {
    auto *weakness = _core->try_get<mods::Weakness<DAMAGE_TYPE>>(event.target);
    auto *resistance = _core->try_get<mods::Resistance<DAMAGE_TYPE>>(event.target);
    auto *immunity = _core->try_get<mods::Immunity<DAMAGE_TYPE>>(event.target);

    auto chance = static_cast<float>(_core->pcg(100));
    if (weakness) {
        chance *= weakness->damage_multiplier;
    }
    if (resistance) {
        chance *= resistance->damage_multiplier;
    }
    if (immunity) {
        chance *= immunity->damage_multiplier;
    }
    if (static_cast<float>(_core->pcg(100)) < chance) {
        _core->registry.emplace_or_replace<AILMENT>(event.target, event.ailment);
    }
};


template<typename DAMAGE_TYPE>
void Combat::on_damage_event(events::battle::DamageEvent<DAMAGE_TYPE> event) {
    auto *weakness = _core->try_get<mods::Weakness<DAMAGE_TYPE>>(event.target);
    auto *resistance = _core->try_get<mods::Resistance<DAMAGE_TYPE>>(event.target);
    auto *immunity = _core->try_get<mods::Immunity<DAMAGE_TYPE>>(event.target);

    auto effective_damage = event.damage.damage;
    if (weakness) {
        effective_damage *= weakness->damage_multiplier;
    }
    if (resistance) {
        effective_damage *= resistance->damage_multiplier;
    }
    if (immunity) {
        effective_damage *= immunity->damage_multiplier;
    }

    auto *attributes = _core->try_get<Attributes>(event.target);
    if (attributes) {
        auto &hit_points = attributes->attributes[character::Attribute::HIT_POINTS];
        hit_points -= effective_damage;
        if (hit_points <= 0) {
            _core->registry.emplace<ailments::Death>(event.target);
        }
    }
    _core->dispatcher.trigger(events::battle::FollowupEvent<DAMAGE_TYPE>{event.target, event.source_skill, event.damage});
};

template<typename DAMAGE_TYPE>
void Combat::followup_attack(events::battle::FollowupEvent<DAMAGE_TYPE> event) {
    DAMAGE_TYPE damage_type;

    _core->registry.view<FollowupAttack>().each([&event, this](auto entity, auto &followup_attack) {
        if (event.source_skill == followup_attack.skill) {
            std::printf("SAME SOURCE Followup attack of type: %s from source %u !\n", DAMAGE_TYPE::description.data(), event.source_skill);
            return;
        } else {
            std::printf("Followup attack of type: %s from source %u !\n", DAMAGE_TYPE::description.data(), event.source_skill);
            _core->dispatcher.trigger(events::battle::AttackEvent{entity, followup_attack.skill});
        }
    });
}

template <typename DAMAGE_TYPE, typename AILMENT>
void Combat::trigger_damage_with_ailment(const entt::entity attacker, const entt::entity target, const entt::entity skill) {

    if (const auto *damage = _core->try_get<DAMAGE_TYPE>(skill)) {
        _core->dispatcher.trigger(events::battle::DamageEvent<DAMAGE_TYPE>{target, skill, *damage});
        _core->dispatcher.trigger(events::battle::AilmentEvent<DAMAGE_TYPE, AILMENT>{target, *damage, _core->registry.get<AILMENT>(skill)});
    }
}

template <typename DAMAGE_TYPE>
void Combat::trigger_damage(const entt::entity attacker, const entt::entity target, const entt::entity skill) {

    if (const auto *damage = _core->try_get<DAMAGE_TYPE>(skill)) {
        _core->dispatcher.trigger(events::battle::DamageEvent<DAMAGE_TYPE>{target, skill, *damage});
    }
}

void Combat::attack(events::battle::AttackEvent event) {
    if (auto target = _core->try_get<targets::TargetSingle>(event.skill)) {
        auto *is_dead = _core->try_get<ailments::Death>(target->target);
        if (is_dead && !is_dead->is_undead) {

        }
        else if (static_cast<float>(_core->pcg(100)) < target->percentage * 100) {
            for (auto damage_type: _attacks[event.skill]) {
                switch (damage_type) {
                    case battle::AttackType::FIRE:
                        trigger_damage_with_ailment<damage_types::FireDmg, ailments::Burn>(event.attacker, target->target, event.skill);
                        break;
                    case battle::AttackType::ICE:
                        trigger_damage<damage_types::IceDmg>(event.attacker, target->target, event.skill);
                        break;
                    case battle::AttackType::PIERCE:
                        trigger_damage<damage_types::PierceDmg>(event.attacker, target->target, event.skill);
                    default:
                        break;
                }
            }
        }
    }
    if (auto *neighbours = _core->try_get<targets::TargetMultiple>(event.skill)) {
        for (auto target: neighbours->targets) {
            auto *is_dead = _core->try_get<ailments::Death>(target);
            if (is_dead && !is_dead->is_undead) {

            }
            else if (static_cast<float>(_core->pcg(100)) < neighbours->percentage * 100) {
                for (auto damage_type: _attacks[event.skill]) {
                    switch (damage_type) {
                        case battle::AttackType::FIRE:
                            trigger_damage_with_ailment<damage_types::FireDmg, ailments::Burn>(event.attacker, target, event.skill);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}