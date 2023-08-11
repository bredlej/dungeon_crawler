//
// Created by Patryk Szczypień on 07/04/2023.
//
#include <level.hpp>

/**
 * @brief Load a level from a given file path
 *
 * This function clears the existing level data, parses the level from the provided JSON file,
 * and initializes the tile and wall maps accordingly. If a player spawn position is specified in
 * the JSON, it updates the player's direction and position in the level.
 *
 * @param path The file path of the JSON level file to load
 */
void Level::load(const std::string &path) {
    clear();
    auto json = LevelParser::parse(path);
    tile_map.from_json(json);
    wall_map.from_json(tile_map, json);
    using namespace level_schema;
    if (json.contains(names[types::player_spawn])) {
        _core->registry.view<components::general::Player, components::general::Direction, components::tiles::MapPosition>().each([&json](const entt::entity entity, const components::general::Player player, components::general::Direction &direction, components::tiles::MapPosition &position) {
            direction.direction = name_to_direction[json[names[types::player_spawn]][names[types::direction]]];
            position.x = json[names[types::player_spawn]][names[types::x]];
            position.y = json[names[types::player_spawn]][names[types::y]];
        });
    }
}

void Level::clear() {
    for (auto wall_entity: wall_map._walls) {
        _core->registry.destroy(wall_entity.entity);
    }
    wall_map._walls.clear();
    for (auto tile_entity: tile_map._tiles) {
        _core->registry.destroy(tile_entity.entity);
    }
    tile_map._tiles.clear();
}

void Level::new_level(int width, int height) {
    clear();
    tile_map._width = width;
    tile_map._height = height;
}
void Level::save(const std::string &path) {
    nlohmann::json json;
    tile_map.to_json(json);
    wall_map.to_json(json);
    using namespace level_schema;
    _core->registry.view<components::general::Player, components::general::Direction, components::tiles::MapPosition>().each([&json](const entt::entity entity, const components::general::Player player, const components::general::Direction &direction, const components::tiles::MapPosition &position) {
        json[names[types::player_spawn]][names[types::direction]] = direction_to_name[direction.direction];
        json[names[types::player_spawn]][names[types::x]] = position.x;
        json[names[types::player_spawn]][names[types::y]] = position.y;
    });
    LevelParser::save(path, json);
}
