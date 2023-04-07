//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_COMPONENTS_HPP
#define DUNGEON_CRAWLER_COMPONENTS_HPP
#include <cstdint>
#include <ecs/types.hpp>
#include <entt/entt.hpp>

namespace components {
    namespace general {
        struct Direction {
            WorldDirection direction;
        };
        struct Player {
            bool is_active;
        };
    }
    namespace values {
        struct EncounterChance {
            float fraction;
        };
        struct Encounter {};
    }
    namespace fields {
        struct MapPosition {
            int32_t x;
            int32_t y;
        };
        struct Field {
            int id;
        };
        struct Floor {
            FloorType type;
        };
        struct Ceiling {
            CeilingType type;
        };
        struct Wall {
            WallType type;
            entt::entity field1;
            entt::entity field2;
        };
        struct Visibility {
            bool is_visible;
        };
        struct Walkability {
            bool walkable;
        };
        struct InFovOfEntity {
            entt::entity entity;
        };
    }
}
#endif//DUNGEON_CRAWLER_COMPONENTS_HPP
