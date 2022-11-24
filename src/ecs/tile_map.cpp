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
    _core->_registry.sort<components::fields::MapPosition>([](const auto &lhs, const auto &rhs) {
        return lhs.x < rhs.x && lhs.y < rhs.y;
    });
}
