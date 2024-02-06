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

static void handle_turn_direction(entt::registry &registry, WorldDirection from_north, WorldDirection from_east, WorldDirection from_south, WorldDirection from_west) {
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

static void change_position(const ModXY mod, entt::registry &registry, const TileMap *tile_map, const WallMap *wall_map, components::tiles::MapPosition &position) {
    using namespace components::tiles;
    if (const entt::entity destination = tile_map->get_at(position.x + mod.x, position.y + mod.y); registry.valid(destination) && registry.any_of<Walkability>(destination) && registry.get<Walkability>(destination).walkable) {
        if (const entt::entity wall = wall_map->get_between(MapPosition{position.x, position.y}, MapPosition{position.x + mod.x, position.y + mod.y}); wall == entt::null || registry.get<Walkability>(wall).walkable) {
            const auto [x, y] = registry.get<MapPosition>(destination);
            position.x = x;
            position.y = y;
        }
    }
}

static void handle_movement(entt::registry &registry, entt::dispatcher &dispatcher, const TileMap *tile_map, const WallMap *wall_map, const ModXY mod_north, const ModXY mod_south, const ModXY mod_west, const ModXY mod_east) {
    for (const auto view = registry.view<components::general::Player, components::tiles::MapPosition, components::general::Direction>(); auto entity: view) {
        auto &position = registry.get<components::tiles::MapPosition>(entity);
        const auto [pos_x, pos_y] = registry.get<components::tiles::MapPosition>(entity);
        switch (const auto direction = registry.get<components::general::Direction>(entity); direction.direction) {
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
        if (pos_x != position.x || pos_y != position.y) {
            dispatcher.enqueue<events::dungeon::Movement>(entity, tile_map->get_at(pos_x, pos_y), tile_map->get_at(position.x, position.y));
        }
    }
    registry.ctx().emplace<events::dungeon::RecalculateFov>();
}

static void open_door(entt::entity player_entity, const ModXY mod, entt::registry &registry, entt::dispatcher &dispatcher, const TileMap *tile_map, const WallMap *wall_map, const components::tiles::MapPosition &position) {
    using namespace components::tiles;
    if (const entt::entity destination = tile_map->get_at(position.x + mod.x, position.y + mod.y); registry.valid(destination)) {
        if (entt::entity wall = wall_map->get_between(MapPosition{position.x, position.y}, MapPosition{position.x + mod.x, position.y + mod.y}); registry.valid(wall) && wall != entt::null) {
            if (const auto door = registry.try_get<Door>(wall)) {
                if (door->state == DoorStateType::CLOSED) {
                    dispatcher.enqueue<events::dungeon::OpenDoor>(player_entity, wall);
                }
            }
        }
    }
}

static auto handle_obstacle_interaction(entt::registry &registry, entt::dispatcher &dispatcher, const TileMap *tile_map, const WallMap *wall_map, const ModXY mod_north, const ModXY mod_south, const ModXY mod_west, const ModXY mod_east) -> void {
    for (const auto view = registry.view<components::general::Player, components::tiles::MapPosition, components::general::Direction>(); const auto player_entity: view) {
        auto &position = registry.get<components::tiles::MapPosition>(player_entity);
        auto old_position = registry.get<components::tiles::MapPosition>(player_entity);
        switch (const auto direction = registry.get<components::general::Direction>(player_entity); direction.direction) {
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

void DungeonActions::turn_left() const {
    handle_turn_direction(_core->registry, WorldDirection::WEST, WorldDirection::NORTH, WorldDirection::EAST, WorldDirection::SOUTH);
}
void DungeonActions::turn_right() const {
    handle_turn_direction(_core->registry, WorldDirection::EAST, WorldDirection::SOUTH, WorldDirection::WEST, WorldDirection::NORTH);
}

void DungeonActions::move_forward() const {
    handle_obstacle_interaction(_core->registry, _core->dispatcher, _tile_map, _wall_map, {0, -1}, {0, 1}, {-1, 0}, {1, 0});
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {0, -1}, {0, 1}, {-1, 0}, {1, 0});
}
void DungeonActions::move_back() const {
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {0, 1}, {0, -1}, {1, 0}, {-1, 0});
}
void DungeonActions::move_left() const {
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {-1, 0}, {1, 0}, {0, 1}, {0, -1});
}
void DungeonActions::move_right() const {
    handle_movement(_core->registry, _core->dispatcher, _tile_map, _wall_map, {1, 0}, {-1, 0}, {0, -1}, {0, 1});
}

void DungeonActions::_on_movement(const events::dungeon::Movement &movement) const {
    const auto [x, y] = _core->registry.get<components::tiles::MapPosition>(movement.to);
    if (const auto *tile_encounter_chance = _core->registry.try_get<components::values::EncounterChance>(_tile_map->get_at(x, y))) {
        const auto encounter_chance = _core->registry.ctx().find<components::values::EncounterChance>();
        _core->dispatcher.enqueue<events::dungeon::EncounterChanceChange>(encounter_chance->chance + tile_encounter_chance->chance);
    }
    if (_core->registry.ctx().contains<components::values::Encounter>()) {
        _core->registry.ctx().erase<components::values::Encounter>();
    }
}

void DungeonActions::_on_open_door(const events::dungeon::OpenDoor &open_door_event) const {
    auto &[type_closed, type_opened, state] = _core->registry.get<components::tiles::Door>(open_door_event.door);
    _core->registry.emplace_or_replace<components::tiles::Walkability>(open_door_event.door, true);
    state = DoorStateType::OPEN;
    _core->game_log.message("You open the door.\n");
}

void DungeonActions::_on_encounter_chance_changed(events::dungeon::EncounterChanceChange encounter_chance_change) const {
    _core->registry.ctx().erase<components::values::EncounterChance>();
    _core->registry.ctx().emplace<components::values::EncounterChance>(encounter_chance_change.fraction);
    if (_core->pcg(100) < static_cast<uint32_t>(encounter_chance_change.fraction * 100)) {
        _core->registry.ctx().erase<components::values::EncounterChance>();
        _core->registry.ctx().emplace<components::values::EncounterChance>(0.0f);
        _core->dispatcher.enqueue<events::dungeon::StartEncounter>();
    }
}

void DungeonActions::start_encounter() const {
    _core->game_log.message("Encounter!\n");
    _core->registry.ctx().emplace<components::values::Encounter>();
}
