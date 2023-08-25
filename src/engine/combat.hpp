//
// Created by Patryk Szczypień on 21/08/2023.
//

#ifndef DUNGEON_CRAWLER_COMBAT_HPP
#define DUNGEON_CRAWLER_COMBAT_HPP
#include <components.hpp>
#include <engine/core.hpp>
#include <events.hpp>
#include <types.hpp>
#include <unordered_map>
using namespace components::battle;

class Combat {
public:
    entt::dispatcher _combat_dispatcher;

    explicit Combat(const std::shared_ptr<Core> &core) : _core{core} {
        initialize();
    };

    void initialize();

    template<typename COMPONENT, battle::AttackType ATTACK_TYPE>
    void register_attack_callback();

    template<typename DAMAGE_TYPE>
    void register_damage_callback();

    template<typename DAMAGE_TYPE, typename AILMENT>
    void register_ailment_callback();

    template<typename DAMAGE_TYPE, typename AILMENT>
    void cause_ailment(events::battle::AilmentEvent<DAMAGE_TYPE, AILMENT> event);

    template<typename DAMAGE_TYPE>
    void cause_damage(events::battle::DamageEvent<DAMAGE_TYPE> event);

    template<typename DAMAGE_TYPE, typename AILMENT>
    void cause_damage_with_ailment(const entt::entity attacker, const entt::entity target, const entt::entity skill);

    template<battle::AttackType T>
    void register_damage(entt::registry &registry, entt::entity entity) {
        _attacks[entity].emplace_back(T);
    }

    void attack(events::battle::AttackEvent event);

    Attributes create_attributes(entt::entity entity) {
        auto &attributes = _core->registry.emplace<Attributes>(entity);
        return attributes;
    }

    void add_attribute(entt::entity entity, character::Attribute attribute, float value) {
        auto &attributes = _core->registry.get<Attributes>(entity);
        attributes.attributes[attribute] = value;
    }

    entt::entity create_character(const std::string &name, float hit_points) {
        auto character = _core->registry.create();
        create_attributes(character);
        add_attribute(character, character::Attribute::HIT_POINTS, hit_points);
        _core->registry.emplace<components::general::Name>(character, name);
        return character;
    }

    void create_fireball(entt::entity attacker, entt::entity main_target, const std::vector<entt::entity> &neighbours) {
        auto fireball = _core->registry.create();
        _core->registry.emplace<components::general::Skill>(fireball);
        _core->registry.emplace<components::general::Name>(fireball, "Fireball");
        _core->registry.emplace<targets::TargetSingle>(fireball, main_target, 1.0f);
        _core->registry.emplace<targets::TargetMultiple>(fireball, neighbours, 0.5f);
        _core->registry.emplace<damage_types::FireDmg>(fireball, 10.0f);
        _core->registry.emplace<ailments::Burn>(fireball, 30.0f, 5.0f, static_cast<uint32_t>(5));

        _combat_dispatcher.trigger(events::battle::AttackEvent{attacker, fireball});

        _core->registry.view<ailments::Burn, Attributes>().each([](auto entity, auto burn, auto attributes) {

        });
    }
private:
    std::shared_ptr<Core> _core;
    std::unordered_map<entt::entity, std::vector<battle::AttackType>> _attacks;
};

#endif//DUNGEON_CRAWLER_COMBAT_HPP
