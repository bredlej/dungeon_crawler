//
// Created by geoco on 29.12.2022.
//
#include <ecs/wall_map.hpp>

entt::entity WallMap::get_between(entt::entity field1, entt::entity field2) const {
    if (field1 == entt::null || field2 == entt::null) return entt::null;
    entt::entity result{entt::null};
    _core->_registry.view<components::fields::Wall>().each([&result, field1, field2](entt::entity entity, const components::fields::Wall &wall) {
        if ((wall.field1 == field1 && wall.field2 == field2) || (wall.field1 == field2 && wall.field2 == field1)) {
            result = entity;
        }
    });
    return result;
}

void WallMap::place_wall_at(const TileMap &tile_map, uint32_t x, uint32_t y, WallType wall_type) {
    auto tile_entity = tile_map.get_at(x,y);
    _core->_registry.emplace_or_replace<components::fields::Walkability>(tile_entity, false);
    auto tile_neighbours = tile_map.get_neighbours_of(x, y);
    for (NeighbourTile neighbour_tile : tile_neighbours) {
        if (get_between(tile_entity, neighbour_tile.entity) == entt::null) {
            entt::entity wall_entity = _core->_registry.create();
            _core->_registry.emplace_or_replace<components::fields::Wall>(wall_entity, wall_type, tile_entity, neighbour_tile.entity);
            _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
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
