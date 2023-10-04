//
// Created by geoco on 24.11.2022.
//

#include "tile_map.hpp"

void TileMap::_initialize() {
    _tiles.clear();
    for (auto y = 0; y < _height; y++) {
        for (auto x = 0; x < _width; x++) {
            Tile tile{_core->registry.create()};
            _core->registry.emplace<components::tiles::MapPosition>(tile.entity, x, y);
            _core->registry.emplace<components::tiles::Floor>(tile.entity, rand() % 100 < 50 ? FloorType::RUINS_01 : FloorType::RUINS_02);
            _core->registry.emplace<components::tiles::Visibility>(tile.entity, true);
            _core->registry.emplace<components::tiles::Walkability>(tile.entity, true);
            _core->registry.emplace<components::tiles::TileId>(tile.entity, 1);
            _core->registry.emplace<components::values::EncounterChance>(tile.entity, 0.05f);
            _tiles.emplace_back(tile);
        }
    }
}

void TileMap::from_json(nlohmann::json &json) {
    using namespace level_schema;
    _tiles.clear();
    int size_x = json[names[schema_types::size_x].data()];
    int size_y = json[names[schema_types::size_y].data()];
    _width = size_x;
    _height = size_y;
    auto tiles = json[names[schema_types::tiles].data()];
    int index = 0;
    for (const auto &tile_contents: tiles) {
        Tile tile{_core->registry.create()};
        _core->registry.emplace<components::tiles::MapPosition>(tile.entity, index % size_x, index / size_y);
        _core->registry.emplace<components::tiles::Visibility>(tile.entity, true);
        _core->registry.emplace<components::tiles::TileId>(tile.entity, 1);
        if (!tile_contents.empty()) {
            for (auto &content: tile_contents) {
                if (content.contains(names[level_schema::schema_types::floor])) {
                    _core->registry.emplace<components::tiles::Floor>(tile.entity, assets::name_to_floor_type[content[names[level_schema::schema_types::floor].data()]]);
                    if (content.contains(names[level_schema::schema_types::walkable])) {
                        _core->registry.emplace<components::tiles::Walkability>(tile.entity, content[names[level_schema::schema_types::walkable].data()]);
                    } else {
                        _core->registry.emplace<components::tiles::Walkability>(tile.entity, true);
                    }
                }
                if (content.contains(names[level_schema::schema_types::ceiling])) {
                    _core->registry.emplace<components::tiles::Ceiling>(tile.entity, assets::name_to_ceiling_type[content[names[level_schema::schema_types::ceiling].data()]]);
                }
                if (content.contains(names[level_schema::schema_types::encounter_chance])) {
                    _core->registry.emplace<components::values::EncounterChance>(tile.entity, content[names[level_schema::schema_types::encounter_chance].data()]);
                }
            }
        } else {
            _core->registry.emplace<components::tiles::Walkability>(tile.entity, false);
        }
        _tiles.emplace_back(tile);
        index += 1;
    }
}

void TileMap::to_json(nlohmann::json &json) {
    using namespace level_schema;
    json[names[schema_types::size_x].data()] = _width;
    json[names[schema_types::size_y].data()] = _height;
    json[names[schema_types::tiles].data()] = nlohmann::json::array();
    for (auto y = 0; y < _height; y++) {
        for (auto x = 0; x < _width; x++) {
            auto tile = get_at(x, y);
            json[names[schema_types::tiles].data()].emplace_back();
            auto &tile_contents = json[names[schema_types::tiles].data()].back();
            tile_contents = nlohmann::json::array();
            std::vector<nlohmann::json> contents;
            if (tile != entt::null) {
                if (_core->registry.any_of<components::tiles::TileId>(tile)) {
                    auto json_tile = nlohmann::json::object();
                    if (_core->registry.any_of<components::tiles::Floor>(tile)) {
                        auto &floor = _core->registry.get<components::tiles::Floor>(tile);
                        json_tile[names[schema_types::floor].data()] = assets::floor_type_to_name[floor.type];
                    }
                    if (_core->registry.any_of<components::tiles::Ceiling>(tile)) {
                        auto &ceiling = _core->registry.get<components::tiles::Ceiling>(tile);
                        json_tile[names[schema_types::ceiling].data()] = assets::ceiling_type_to_name[ceiling.type];
                    }
                    if (_core->registry.any_of<components::tiles::Walkability>(tile)) {
                        auto &walkability = _core->registry.get<components::tiles::Walkability>(tile);
                        json_tile[names[schema_types::walkable].data()] = walkability.walkable;
                    }
                    if (components::values::EncounterChance *encounter_chance = _core->registry.try_get<components::values::EncounterChance>(tile)) {
                        json_tile[names[schema_types::encounter_chance].data()] = encounter_chance->chance;//fmt::format("{:.2f}", encounter_chance->chance);
                    }
                    contents.emplace_back(json_tile);
                }
            }
            tile_contents = contents;
        }
    }
}

entt::entity TileMap::get_at(int32_t x, int32_t y) const {
    entt::entity result{entt::null};
    _core->registry.view<components::tiles::TileId, components::tiles::MapPosition>().each([&result, &x, &y](entt::entity entity, const components::tiles::TileId &, const components::tiles::MapPosition &position) {
        if (position.x == x && position.y == y) {
            result = entity;
        }
    });
    return result;
}

std::vector<NeighbourTile> TileMap::get_neighbours_of(int32_t x, int32_t y) const {
    std::vector<NeighbourTile> result;
    entt::entity neighbour = get_at(x - 1, y);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::WEST});
    }
    neighbour = get_at(x + 1, y);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::EAST});
    }
    neighbour = get_at(x, y - 1);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::NORTH});
    }
    neighbour = get_at(x, y + 1);
    if (neighbour != entt::null) {
        result.emplace_back(NeighbourTile{neighbour, WorldDirection::SOUTH});
    }
    return result;
}
