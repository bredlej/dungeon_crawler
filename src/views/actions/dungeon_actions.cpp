#include "dungeon_actions.hpp"
#include "dungeon_view.hpp"
#include "tile_map.hpp"
#include "wall_map.hpp"

//
// Created by Patryk Szczypień on 23/03/2023.
//
void DungeonActions::_initialize() {
    _core->_dispatcher.sink<events::dungeon::TurnLeft>().connect<&DungeonActions::turn_left>(this);
    _core->_dispatcher.sink<events::dungeon::TurnRight>().connect<&DungeonActions::turn_right>(this);
    _core->_dispatcher.sink<events::dungeon::MoveForward>().connect<&DungeonActions::move_forward>(this);
    _core->_dispatcher.sink<events::dungeon::MoveBack>().connect<&DungeonActions::move_back>(this);
    _core->_dispatcher.sink<events::dungeon::MoveLeft>().connect<&DungeonActions::move_left>(this);
    _core->_dispatcher.sink<events::dungeon::MoveRight>().connect<&DungeonActions::move_right>(this);
}

static inline void handle_turn_direction (entt::registry &registry, WorldDirection from_north, WorldDirection from_east, WorldDirection from_south, WorldDirection from_west) {
    registry.view<components::general::Player, components::general::Direction>().each([from_north, from_east, from_south, from_west](const entt::entity entity, const components::general::Player player, components::general::Direction &direction) {
        switch (direction.direction) {
            case WorldDirection::NORTH: direction.direction = from_north; break;
            case WorldDirection::EAST: direction.direction = from_east; break;
            case WorldDirection::SOUTH: direction.direction = from_south; break;
            case WorldDirection::WEST: direction.direction = from_west; break;
        }
    });
    registry.ctx().emplace<events::dungeon::RecalculateFov>();
}

static inline void change_position(ModXY mod, entt::registry &registry, const TileMap *tile_map, const WallMap *wall_map, components::fields::MapPosition &position) {
    entt::entity destination = tile_map->get_at(position.x + mod.x, position.y + mod.y);
    if (registry.valid(destination) && registry.get<components::fields::Walkability>(destination).is_walkable) {
        entt::entity wall = wall_map->get_between(tile_map->get_at(position.x, position.y), destination);
        if (wall == entt::null || registry.get<components::fields::Walkability>(wall).is_walkable) {
            components::fields::MapPosition destination_position = registry.get<components::fields::MapPosition>(destination);
            position.x = destination_position.x;
            position.y = destination_position.y;
        }
    }
}

static inline void handle_movement (entt::registry &registry, const TileMap *tile_map, const WallMap *wall_map, ModXY mod_north, ModXY mod_south, ModXY mod_west, ModXY mod_east) {
    auto view = registry.view<components::general::Player, components::fields::MapPosition, components::general::Direction>();
    for (auto entity: view) {
        auto &position = registry.get<components::fields::MapPosition>(entity);
        auto direction = registry.get<components::general::Direction>(entity);
        switch (direction.direction) {
            case WorldDirection::NORTH: change_position(mod_north, registry, tile_map, wall_map, position); break;
            case WorldDirection::SOUTH: change_position(mod_south, registry, tile_map, wall_map, position); break;
            case WorldDirection::WEST: change_position(mod_west, registry, tile_map, wall_map, position); break;
            case WorldDirection::EAST: change_position(mod_east, registry, tile_map, wall_map, position); break;
        }
    }
    registry.ctx().emplace<events::dungeon::RecalculateFov>();
    auto encounter_chance = registry.ctx().find<components::values::EncounterChance>();
    components::values::EncounterChance new_chance {encounter_chance->fraction + 0.05f};
    registry.ctx().erase<components::values::EncounterChance>();
    registry.ctx().emplace<components::values::EncounterChance>(new_chance.fraction);
}

void DungeonActions::turn_left() {
    handle_turn_direction(_core->_registry, WorldDirection::WEST, WorldDirection::NORTH, WorldDirection::EAST, WorldDirection::SOUTH);
}
void DungeonActions::turn_right() {
    handle_turn_direction(_core->_registry, WorldDirection::EAST, WorldDirection::SOUTH, WorldDirection::WEST, WorldDirection::NORTH);
}

void DungeonActions::move_forward() {
    handle_movement(_core->_registry, _tile_map, _wall_map, {0, -1}, {0, 1}, {-1, 0}, {1, 0});
}
void DungeonActions::move_back() {
    handle_movement(_core->_registry, _tile_map, _wall_map, {0, 1}, {0, -1}, {1, 0}, {-1, 0});
}
void DungeonActions::move_left() {
    handle_movement(_core->_registry, _tile_map, _wall_map, {-1, 0}, {1, 0}, {0, 1}, {0, -1});
}
void DungeonActions::move_right() {
    handle_movement(_core->_registry, _tile_map, _wall_map, {1, 0}, {-1, 0}, {0, -1}, {0, 1});
}
