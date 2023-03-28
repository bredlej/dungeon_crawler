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
#endif//DUNGEON_CRAWLER_EVENTS_HPP
