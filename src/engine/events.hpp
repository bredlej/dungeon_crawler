//
// Created by Patryk Szczypień on 20/03/2023.
//

#ifndef DUNGEON_CRAWLER_EVENTS_HPP
#define DUNGEON_CRAWLER_EVENTS_HPP

namespace events::ui {
    struct ToggleShowDemo {};
}
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
    struct StartEncounter {};
}

namespace events::battle {
    struct AttackEvent {
        entt::entity attacker;
        entt::entity skill;
    };

    template <typename DAMAGE_TYPE>
    struct DamageEvent {
        entt::entity source_skill;
        entt::entity target;
        DAMAGE_TYPE damage;
    };

    template <typename DAMAGE_TYPE>
    struct FollowupEvent {
        entt::entity source_skill;
        entt::entity target;
        DAMAGE_TYPE damage;
    };

    template <typename DAMAGE_TYPE, typename AILMENT>
    struct AilmentEvent {
        entt::entity target;
        DAMAGE_TYPE damage;
        AILMENT ailment;
    };
}
#endif//DUNGEON_CRAWLER_EVENTS_HPP
