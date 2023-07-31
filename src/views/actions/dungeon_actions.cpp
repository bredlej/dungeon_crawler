#include "dungeon_actions.hpp"


//
// Created by Patryk Szczypień on 23/03/2023.
//
void DungeonActions::_initialize() {
    _core->dispatcher.sink<events::dungeon::TurnLeft>().connect<&DungeonActions::turn_left>(this);
    _core->dispatcher.sink<events::dungeon::TurnRight>().connect<&DungeonActions::turn_right>(this);
    _core->dispatcher.sink<events::dungeon::MoveForward>().connect<&DungeonActions::move_forward>(this);
    _core->dispatcher.sink<events::dungeon::MoveBack>().connect<&DungeonActions::move_back>(this);
    _core->dispatcher.sink<events::dungeon::MoveLeft>().connect<&DungeonActions::move_left>(this);
    _core->dispatcher.sink<events::dungeon::MoveRight>().connect<&DungeonActions::move_right>(this);
    _core->dispatcher.sink<events::dungeon::Movement>().connect<&DungeonActions::_on_movement>(this);
    _core->dispatcher.sink<events::dungeon::OpenDoor>().connect<&DungeonActions::_on_open_door>(this);
    _core->dispatcher.sink<events::dungeon::EncounterChanceChange>().connect<&DungeonActions::_on_encounter_chance_changed>(this);
    _core->dispatcher.sink<events::dungeon::StartEncounter>().connect<&DungeonActions::start_encounter>(this);
}

static inline void handle_turn_direction(entt::registry &registry, WorldDirection from_north, WorldDirection from_east, WorldDirection from_south, WorldDirection from_west) {
    registry.view<components::general::Player, components::general::Direction>().each([from_north, from_east, from_south, from_west](const entt::entity entity, const components::general::Player player, components::general::Direction &direction) {
        switch (direction.direction) {
            case WorldDirection::NORTH:
                direction.direction = from_north;
                break;
            case WorldDirection::EAST:
                direction.direction = from_east;
                break;
            case WorldDirection::SOUTH:
                direction.direction = from_south;
                break;
            case WorldDirection::WEST:
                direction.direction = from_west;
                break;
        }
    });
    registry.ctx().emplace<events::dungeon::RecalculateFov>();
}

static inline void change_position(ModXY mod, entt::registry &registry, const TileMap *tile_map, const WallMap *wall_map, components::tiles::MapPosition &position) {
    using namespace components::tiles;
    entt::entity destination = tile_map->get_at(position.x + mod.x, position.y + mod.y);
    if (registry.valid(destination) && registry.get<components::tiles::Walkability>(destination).walkable) {
        entt::entity wall = wall_map->get_between(MapPosition{position.x, position.y}, MapPosition{position.x + mod.x, position.y + mod.y});
        if (wall == entt::null || registry.get<components::tiles::Walkability>(wall).walkable) {
            components::tiles::MapPosition destination_position = registry.get<components::tiles::MapPosition>(destination);
            position.x = destination_position.x;
            position.y = destination_position.y;
        }
    }
}

static inline void handle_movement(entt::registry &registry, entt::dispatcher &dispatcher, const TileMap *tile_map, const WallMap *wall_map, ModXY mod_north, ModXY mod_south, ModXY mod_west, ModXY mod_east) {
    auto view = registry.view<components::general::Player, components::tiles::MapPosition, components::general::Direction>();
    for (auto entity: view) {
        auto &position = registry.get<components::tiles::MapPosition>(entity);
        auto old_position = registry.get<components::tiles::MapPosition>(entity);
        auto direction = registry.get<components::general::Direction>(entity);
        switch (direction.direction) {
            case WorldDirection::NORTH: {
                change_position(mod_north, registry, tile_map, wall_map, position);
                break;
            }
            case WorldDirection::SOUTH:
                change_position(mod_south, registry, tile_map, wall_map, position);
                break;
            case WorldDirection::WEST:
                change_position(mod_west, registry, tile_map, wall_map, position);
                break;
            case WorldDirection::EAST:
                change_position(mod_east, registry, tile_map, wall_map, position);
                break;
        }
        if (old_position.x != position.x || old_position.y != position.y) {
            dispatcher.enqueue<events::dungeon::Movement>(entity, tile_map->get_at(old_position.x, old_position.y), tile_map->get_at(position.x, position.y));
        }
    }
    registry.ctx().emplace<events::dungeon::RecalculateFov>();
}
static inline void open_door(entt::entity player_entity, ModXY mod, entt::registry &registry, entt::dispatcher &dispatcher, const TileMap *tile_map, const WallMap *wall_map, components::tiles::MapPosition &position) {
    using namespace components::tiles;
    entt::entity destination = tile_map->get_at(position.x + mod.x, position.y + mod.y);
    if (registry.valid(destination)) {
        entt::entity wall = wall_map->get_between(MapPosition{position.x, position.y}, MapPosition{position.x + mod.x, position.y + mod.y});
        if (registry.valid(wall) && wall != entt::null) {
            if (auto door = registry.try_get<components::tiles::Door>(wall)) {
                if (door->state == DoorStateType::CLOSED) {
                    dispatcher.enqueue<events::dungeon::OpenDoor>(player_entity, wall);
                }
            }
        }
    }
}

static inline void handle_obstacle_interaction(entt::registry &registry, entt::dispatcher &dispatcher, const TileMap *tile_map, const WallMap *wall_map, ModXY mod_north, ModXY mod_south, ModXY mod_west, ModXY mod_east) {
    auto view = registry.view<components::general::Player, components::tiles::MapPosition, components::general::Direction>();
    for (auto player_entity: view) {
        auto &position = registry.get<components::tiles::MapPosition>(player_entity);
        auto old_position = registry.get<components::tiles::MapPosition>(player_entity);
        auto direction = registry.get<components::general::Direction>(player_entity);
        switch (direction.direction) {
            case WorldDirection::NORTH: {
                open_door(player_entity, mod_north, registry, dispatcher, tile_map, wall_map, position);
                break;
            }
            case WorldDirection::SOUTH:
                open_door(player_entity, mod_south, registry, dispatcher, tile_map, wall_map, position);
                break;
            case WorldDirection::WEST:
                open_door(player_entity, mod_west, registry, dispatcher, tile_map, wall_map, position);
                break;
            case WorldDirection::EAST:
                open_door(player_entity, mod_east, registry, dispatcher, tile_map, wall_map, position);
                break;
        }
    }
    registry.ctx().emplace<events::dungeon::RecalculateFov>();
}

void DungeonActions::turn_left() {
    handle_turn_direction(_core->registry, WorldDirection::WEST, WorldDirection::NORTH, WorldDirection::EAST, WorldDirection::SOUTH);
}
void DungeonActions::turn_right() {
    handle_turn_direction(_core->registry, WorldDirection::EAST, WorldDirection::SOUTH, WorldDirection::WEST, WorldDirection::NORTH);
}

void DungeonActions::move_forward() {
    handle_obstacle_interaction(_core->registry, _core->dispatcher, _tile_map, _wall_map, {0, -1}, {0, 1}, {-1, 0}, {1, 0});
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {0, -1}, {0, 1}, {-1, 0}, {1, 0});
}
void DungeonActions::move_back() {
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {0, 1}, {0, -1}, {1, 0}, {-1, 0});
}
void DungeonActions::move_left() {
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {-1, 0}, {1, 0}, {0, 1}, {0, -1});
}
void DungeonActions::move_right() {
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {1, 0}, {-1, 0}, {0, -1}, {0, 1});
}

void DungeonActions::_on_movement(const events::dungeon::Movement &movement) {
    components::tiles::MapPosition new_position = _core->registry.get<components::tiles::MapPosition>(movement.to);
    auto *tile_encounter_chance = _core->registry.try_get<components::values::EncounterChance>(_tile_map->get_at(new_position.x, new_position.y));
    if (tile_encounter_chance) {
        auto encounter_chance = _core->registry.ctx().find<components::values::EncounterChance>();
        _core->dispatcher.enqueue<events::dungeon::EncounterChanceChange>(encounter_chance->chance + tile_encounter_chance->chance);
    }
    if (_core->registry.ctx().contains<components::values::Encounter>()) {
        _core->registry.ctx().erase<components::values::Encounter>();
    }
}

void DungeonActions::_on_open_door(const events::dungeon::OpenDoor &open_door_event) {
    auto &door = _core->registry.get<components::tiles::Door>(open_door_event.door);
    _core->registry.emplace_or_replace<components::tiles::Walkability>(open_door_event.door, true);
    door.state = DoorStateType::OPEN;
    _core->game_log.message("You open the door.\n");
    _core->dispatcher.enqueue<events::dungeon::RecalculateFov>();
}

void DungeonActions::_on_encounter_chance_changed(events::dungeon::EncounterChanceChange encounter_chance_change) {
    _core->registry.ctx().erase<components::values::EncounterChance>();
    _core->registry.ctx().emplace<components::values::EncounterChance>(encounter_chance_change.fraction);
    if (_core->pcg(100) < static_cast<uint32_t>(encounter_chance_change.fraction * 100)) {
        _core->registry.ctx().erase<components::values::EncounterChance>();
        _core->registry.ctx().emplace<components::values::EncounterChance>(0.0f);
        _core->dispatcher.enqueue<events::dungeon::StartEncounter>();
    }
}

void DungeonActions::start_encounter() {
    _core->game_log.message("Encounter!\n");
    _core->registry.ctx().emplace<components::values::Encounter>();
}
