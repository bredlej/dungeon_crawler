//
// Created by geoco on 24.11.2022.
//

#include "tile_map.hpp"

void TileMap::_initialize() {
    _tiles.clear();
    for (auto y = 0; y < _height; y++) {
        for (auto x = 0; x < _width; x++) {
            Tile tile{_core->_registry.create()};
            _core->_registry.emplace<components::fields::MapPosition>(tile.entity, x, y);
            _core->_registry.emplace<components::fields::Floor>(tile.entity, FloorType::RUINS_01);
            _core->_registry.emplace<components::fields::Ceiling>(tile.entity, CeilingType::NORMAL);
            _core->_registry.emplace<components::fields::Visibility>(tile.entity, true);
            _core->_registry.emplace<components::fields::Walkability>(tile.entity, true);
            _tiles.emplace_back(tile);
        }
    }
}
entt::entity TileMap::get_at(int32_t x, int32_t y) const {
    entt::entity result{entt::null};
    _core->_registry.view<components::fields::MapPosition>().each([&result, &x, &y](entt::entity entity, const components::fields::MapPosition &position) {
        if (position.x == x && position.y == y) {
            result = entity;
        }
    });
    return result;
}

std::vector<NeighbourTile> TileMap::get_neighbours_of(int32_t x, int32_t y) const {
    std::vector<NeighbourTile> result(4);
    entt::entity neighbour = get_at(x-1, y);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::WEST});
    }
    neighbour = get_at(x+1, y);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::EAST});
    }
    neighbour = get_at(x, y-1);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::NORTH});
    }
    neighbour = get_at(x, y+1);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::SOUTH});
    }
    return result;
}
