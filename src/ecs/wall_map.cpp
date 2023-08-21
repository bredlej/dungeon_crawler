//
// Created by geoco on 29.12.2022.
//
#include <ecs/wall_map.hpp>

entt::entity WallMap::get_between(components::tiles::MapPosition field1, components::tiles::MapPosition field2) const noexcept {
    entt::entity result{entt::null};
    _core->registry.view<components::tiles::Wall>().each([&result, field1, field2](entt::entity entity, const components::tiles::Wall &wall) {
        if ((wall.field1.x == field1.x && wall.field1.y == field1.y && wall.field2.x == field2.x && wall.field2.y == field2.y) ||
            (wall.field1.x == field2.x && wall.field1.y == field2.y && wall.field2.x == field1.x && wall.field2.y == field1.y)) {
            result = entity;
        }
    });
    return result;
}

void WallMap::place_wall_at(const TileMap &tile_map, int32_t x, int32_t y, WallType wall_type) {
    auto tile_entity = tile_map.get_at(x,y);
    _core->registry.emplace_or_replace<components::tiles::Walkability>(tile_entity, false);
    auto tile_neighbours = tile_map.get_neighbours_of(x, y);
    for (NeighbourTile neighbour_tile : tile_neighbours) {
        components::tiles::MapPosition neighbour_position = _core->registry.get<components::tiles::MapPosition>(neighbour_tile.entity);
        if (get_between(components::tiles::MapPosition{x, y}, neighbour_position) == entt::null) {
            entt::entity wall_entity = _core->registry.create();
            _core->registry.emplace_or_replace<components::tiles::Wall>(wall_entity, wall_type, components::tiles::MapPosition{x, y}, neighbour_position);
            _core->registry.emplace_or_replace<components::tiles::Walkability>(wall_entity, false);
            _walls.emplace_back(WallEntity{wall_entity});
        }
    }
}

static constexpr inline std::array<uint8_t, 400> walls = {
        0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,
        0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,
        0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
        0,0,1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,
        0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,
        0,0,1,1,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,1,0,1,1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void WallMap::initialize(const TileMap &tile_map) {
    for (size_t i = 0; i < walls.size(); i++) {
        if (walls[i] == 1) {
            place_wall_at(tile_map, i % 20, i / 20, WallType::RUINS_01);
        }
    }
}

void WallMap::from_json(const TileMap &tile_map, const nlohmann::json &json) {
    using namespace level_schema;
    for (const auto &wall: json[names[types::walls].data()]) {
        WallType wall_type = assets::name_to_wall_type[wall[names[types::wall].data()]];
        components::tiles::MapPosition field1_pos = {wall[names[types::between].data()][0], wall[names[types::between].data()][1]};
        components::tiles::MapPosition field2_pos = {wall[names[types::between].data()][2], wall[names[types::between].data()][3]};
        if (get_between(field1_pos, field2_pos) == entt::null) {
            entt::entity wall_entity = _core->registry.create();
            _core->registry.emplace_or_replace<components::tiles::Wall>(wall_entity, wall_type, field1_pos, field2_pos);
            _core->registry.emplace_or_replace<components::tiles::Walkability>(wall_entity, false);
            _walls.emplace_back(WallEntity{wall_entity});
            if (wall.contains(names[types::door])) {
                auto door_closed_type = assets::name_to_door_type[wall[names[types::door].data()][names[types::type_closed].data()]];
                auto door_open_type = assets::name_to_door_type[wall[names[types::door].data()][names[types::type_opened].data()]];
                auto door_state = assets::name_to_door_state_type[wall[names[types::door].data()][names[types::state].data()]];
                _core->registry.emplace_or_replace<components::tiles::Door>(wall_entity, door_closed_type, door_open_type, door_state);
                if (door_state == DoorStateType::OPEN) {
                    _core->registry.emplace_or_replace<components::tiles::Walkability>(wall_entity, true);
                }
            }
        }
    }
}
void WallMap::to_json(nlohmann::json &json) {
    using namespace level_schema;
    json[names[types::walls].data()] = nlohmann::json::array();
    for (const auto &wall: _walls) {
        auto &wall_component = _core->registry.get<components::tiles::Wall>(wall.entity);
        auto door_component = _core->registry.try_get<components::tiles::Door>(wall.entity);
        // clang-format off
        json[names[types::walls].data()].push_back({
            {names[types::wall], assets::wall_type_to_name[wall_component.type]},
            {names[types::between], {
                wall_component.field1.x,
                wall_component.field1.y,
                wall_component.field2.x,
                wall_component.field2.y
            }},
        });
        if (door_component) {
            json[names[types::walls].data()].back().push_back({
                names[types::door], {
                    {names[types::type_opened], assets::door_type_to_name[door_component->type_opened]},
                    {names[types::type_closed], assets::door_type_to_name[door_component->type_closed]},
                    {names[types::state], assets::door_state_type_to_name[door_component->state]},
                }
            });
        }
        // clang-format on
    }
}
