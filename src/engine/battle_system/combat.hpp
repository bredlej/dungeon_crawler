//
// Created by Patryk Szczypień on 21/08/2023.
//

#ifndef DUNGEON_CRAWLER_COMBAT_HPP
#define DUNGEON_CRAWLER_COMBAT_HPP
#include "ecs/components.hpp"
#include "ecs/types.hpp"
#include "engine/core.hpp"
#include "engine/events.hpp"
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
    void on_damage_event(events::battle::DamageEvent<DAMAGE_TYPE> event);

    template<typename DAMAGE_TYPE>
    void followup_attack(events::battle::FollowupEvent<DAMAGE_TYPE> event);

    template<typename DAMAGE_TYPE, typename AILMENT>
    void trigger_damage_with_ailment(const entt::entity attacker, const entt::entity target, const entt::entity skill);

    template<typename DAMAGE_TYPE>
    void trigger_damage(const entt::entity attacker, const entt::entity target, const entt::entity skill);

    template<battle::AttackType T>
    void register_damage(entt::registry &registry, entt::entity entity) {
        _attacks[entity].emplace_back(T);
    }

    void attack(events::battle::AttackEvent event);


private:
    std::shared_ptr<Core> _core;
    std::unordered_map<entt::entity, std::vector<battle::AttackType>> _attacks;
};

#endif//DUNGEON_CRAWLER_COMBAT_HPP
