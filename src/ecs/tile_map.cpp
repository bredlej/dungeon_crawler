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
            _core->_registry.emplace<components::fields::Floor>(tile.entity, FloorType::NORMAL);
            _core->_registry.emplace<components::fields::Ceiling>(tile.entity, CeilingType::NORMAL);
            _core->_registry.emplace<components::fields::Visibility>(tile.entity, true);
            _core->_registry.emplace<components::fields::Walkability>(tile.entity, true);
            _tiles.emplace_back(tile);
        }
    }
    auto wall_entity = get_at(5,5);
    _core->_registry.emplace<components::fields::Wall>(wall_entity, WallType::NORMAL);
    _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
    wall_entity = get_at(5,4);
    _core->_registry.emplace<components::fields::Wall>(wall_entity, WallType::NORMAL);
    _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
    wall_entity = get_at(5,3);
    _core->_registry.emplace<components::fields::Wall>(wall_entity, WallType::NORMAL);
    _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
    wall_entity = get_at(4,3);
    _core->_registry.emplace<components::fields::Wall>(wall_entity, WallType::NORMAL);
    _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
    wall_entity = get_at(3,3);
    _core->_registry.emplace<components::fields::Wall>(wall_entity, WallType::NORMAL);
    _core->_registry.emplace_or_replace<components::fields::Walkability>(wall_entity, false);
}
entt::entity TileMap::get_at(int32_t x, int32_t y) const {
    entt::entity result{entt::null};
    _core->_registry.view<components::fields::MapPosition>().each([&result, &x, &y](entt::entity entity, components::fields::MapPosition position) {
        if (position.x == x && position.y == y) {
            result = entity;
        }
    });
    return result;
}
