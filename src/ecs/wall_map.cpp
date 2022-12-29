//
// Created by geoco on 29.12.2022.
//
#include <ecs/wall_map.hpp>

entt::entity WallMap::get_between(entt::entity field1, entt::entity field2) const {
    if (field1 == entt::null || field2 == entt::null) return entt::null;
    entt::entity result{entt::null};
    _core->_registry.view<components::fields::Wall>().each([&result, field1, field2](entt::entity entity, const components::fields::Wall &wall) {
        if (wall.field1 == field1 && wall.field2 == field2) {
            result = entity;
        }
    });
    return result;
}
void WallMap::initialize(const TileMap &tile_map) {
    auto tile_entity = tile_map.get_at(5,5);
    auto tile_neighbours = tile_map.get_neighbours_of(5, 5);
    for (NeighbourTile neighbour_tile : tile_neighbours) {
        auto wall_entity = _core->_registry.create();
        _core->_registry.emplace_or_replace<components::fields::Wall>(wall_entity, WallType::RUINS_01, tile_entity, neighbour_tile.entity);
        _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
        _walls.emplace_back(WallEntity{wall_entity});
        components::fields::MapPosition position = _core->_registry.get<components::fields::MapPosition>(neighbour_tile.entity);
        std::printf("Added wall between 5,5 and %d, %d\n", position.x, position.y);
    }

}
