//
// Created by Patryk Szczypień on 07/04/2023.
//
#include <level.hpp>

void Level::load(const std::string &path) {
    auto json = LevelParser::parse(path);
    tile_map.from_json(json);
    wall_map.from_json(tile_map, json);
    using namespace level_schema;
    if (json.contains(names[types::player_spawn])) {
        _core->registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>().each([&json](const entt::entity entity, const components::general::Player player, components::general::Direction &direction, components::fields::MapPosition &position) {
            direction.direction = direction_names[json[names[types::player_spawn]][names[types::direction]]];
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
void Level::save(const std::string &path) {
}
