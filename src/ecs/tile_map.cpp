//
// Created by geoco on 24.11.2022.
//

#include "tile_map.hpp"

void TileMap::_initialize() {
    _tiles.clear();
    for (auto y = 0; y < _height; y++) {
        for (auto x = 0; x < _width; x++) {
            Tile tile{_core->registry.create()};
            _core->registry.emplace<components::fields::MapPosition>(tile.entity, x, y);
            _core->registry.emplace<components::fields::Floor>(tile.entity, rand() % 100 < 50 ? FloorType::RUINS_01 : FloorType::RUINS_02);
            _core->registry.emplace<components::fields::Ceiling>(tile.entity, CeilingType::NORMAL);
            _core->registry.emplace<components::fields::Visibility>(tile.entity, true);
            _core->registry.emplace<components::fields::Walkability>(tile.entity, true);
            _core->registry.emplace<components::fields::Field>(tile.entity, 1);
            _core->registry.emplace<components::values::EncounterChance>(tile.entity, 0.05f);
            _tiles.emplace_back(tile);
        }
    }
}

void TileMap::from_json(nlohmann::json &json) {
    using namespace level_schema;
    _tiles.clear();
    int size_x = json[names[types::size_x]];
    int size_y = json[names[types::size_x]];
    auto tiles = json[names[types::tiles]];
    int index = 0;
    for (const auto &tile_contents: tiles) {
        Tile tile{_core->registry.create()};
        _core->registry.emplace<components::fields::MapPosition>(tile.entity, index % size_x, index / size_y);
        _core->registry.emplace<components::fields::Ceiling>(tile.entity, CeilingType::NORMAL);
        _core->registry.emplace<components::fields::Visibility>(tile.entity, true);
        _core->registry.emplace<components::fields::Field>(tile.entity, 1);
        if (!tile_contents.empty()) {
            for (auto &content: tile_contents) {
                if (content.contains(names[level_schema::types::floor])) {
                    _core->registry.emplace<components::fields::Floor>(tile.entity, assets::name_to_floor_type[content[names[level_schema::types::floor]]]);
                    if (content.contains(names[level_schema::types::walkable])) {
                        _core->registry.emplace<components::fields::Walkability>(tile.entity, content[names[level_schema::types::walkable]]);
                    }
                    else {
                        _core->registry.emplace<components::fields::Walkability>(tile.entity, true);
                    }
                }
                if (content.contains(names[level_schema::types::encounter_chance])) {
                    _core->registry.emplace<components::values::EncounterChance>(tile.entity, content[names[level_schema::types::encounter_chance]]);
                }
            }
        } else {
            _core->registry.emplace<components::fields::Walkability>(tile.entity, true);
        }
        _tiles.emplace_back(tile);
        index += 1;
    }
}

void TileMap::to_json(nlohmann::json &json) {
    using namespace level_schema;
    json[names[types::size_x]] = _width;
    json[names[types::size_y]] = _height;
    json[names[types::tiles]] = nlohmann::json::array();
    for (auto y = 0; y < _height; y++) {
        for (auto x = 0; x < _width; x++) {
            auto tile = get_at(x, y);
            if (tile != entt::null) {
                json[names[types::tiles]].emplace_back();
                auto &tile_contents = json[names[types::tiles]].back();
                tile_contents = nlohmann::json::array();
                if (_core->registry.any_of<components::fields::Floor>(tile)) {
                    std::vector<nlohmann::json> contents;
                    auto json_floor = nlohmann::json::object();
                    auto &floor = _core->registry.get<components::fields::Floor>(tile);
                    json_floor[names[types::floor]] = assets::floor_type_to_name[floor.type];
                    if (_core->registry.any_of<components::fields::Walkability>(tile)) {
                        auto &walkability = _core->registry.get<components::fields::Walkability>(tile);
                        json_floor[names[types::walkable]] = walkability.walkable;
                    }
                    json_floor[names[types::encounter_chance]] = _core->registry.get<components::values::EncounterChance>(tile).chance;
                    contents.emplace_back(json_floor);
                    tile_contents=contents;
                }
            }
        }
    }
}

entt::entity TileMap::get_at(int32_t x, int32_t y) const {
    entt::entity result{entt::null};
    _core->registry.view<components::fields::Field, components::fields::MapPosition>().each([&result, &x, &y](entt::entity entity, const components::fields::Field &, const components::fields::MapPosition &position) {
        if (position.x == x && position.y == y) {
            result = entity;
        }
    });
    return result;
}

std::vector<NeighbourTile> TileMap::get_neighbours_of(int32_t x, int32_t y) const {
    std::vector<NeighbourTile> result;
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
