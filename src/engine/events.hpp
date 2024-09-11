//
// Created by Patryk Szczypień on 20/03/2023.
//

#ifndef DUNGEON_CRAWLER_EVENTS_HPP
#define DUNGEON_CRAWLER_EVENTS_HPP
#include <ecs/types.hpp>
#include <entt/entt.hpp>

namespace events::ui {
    struct ToggleShowDemo {};
    struct ShowPartyView {};
    struct ShowDungeonView {};
    namespace party {
        struct NextPartyMember {};
        struct PreviousPartyMember {};
    }// namespace party
}// namespace events::ui
namespace events::dungeon {
    struct RecalculateFov {};
    struct TurnLeft {};
    struct TurnRight {};
    struct MoveForward {};
    struct MoveBack {};
    struct MoveLeft {};
    struct MoveRight {};
    struct OpenDoor {
        entt::entity who;
        entt::entity door;
    };
    struct Movement {
        entt::entity who;
        entt::entity from;
        entt::entity to;
    };
    struct EncounterChanceChange {
        float fraction;
    };

    struct StartEncounter {
    };

    struct EndEncounter {
    };
}// namespace events::dungeon

namespace events::battle {
    struct NextStateEvent {
        types::battle::BattlePhase from_phase;
    };
    struct AttackEvent {
        entt::entity attacker;
        entt::entity skill;
    };

    template<typename DAMAGE_TYPE>
    struct DamageEvent {
        entt::entity source_skill;
        entt::entity target;
        DAMAGE_TYPE damage;
    };

    template<typename DAMAGE_TYPE>
    struct FollowupEvent {
        entt::entity source_skill;
        entt::entity target;
        DAMAGE_TYPE damage;
    };

    template<typename DAMAGE_TYPE, typename AILMENT>
    struct AilmentEvent {
        entt::entity target;
        DAMAGE_TYPE damage;
        AILMENT ailment;
    };
}// namespace events::battle
#endif//DUNGEON_CRAWLER_EVENTS_HPP
