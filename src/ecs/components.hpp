//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_COMPONENTS_HPP
#define DUNGEON_CRAWLER_COMPONENTS_HPP
#include <cstdint>
#include <ecs/types.hpp>

namespace components {
    namespace general {
        struct Direction {
            WorldDirection direction;
        };
        struct Player {
            bool is_active;
        };
    }
    namespace fields {
        struct MapPosition {
            int32_t x;
            int32_t y;
        };
        struct Floor {
            FloorType type;
        };
        struct Ceiling {
            CeilingType type;
        };
        struct Wall {
            WallType type;
        };
        struct Visibility {
            bool is_visible;
        };
        struct Walkability {
            bool is_walkable;
        };
    }
}
#endif//DUNGEON_CRAWLER_COMPONENTS_HPP
