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
            float chance;
        };
        struct Encounter {};
        struct Tint {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
    }
    namespace tiles {
        struct MapPosition {
            int32_t x;
            int32_t y;
        };
        struct TileId {
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
            MapPosition field1;
            MapPosition field2;
        };
        struct Door {
            DoorType type_closed;
            DoorType type_opened;
            DoorStateType state;
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
