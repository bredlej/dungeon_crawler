//
// Created by Patryk Szczypień on 09/03/2023.
//

#ifndef DUNGEON_CRAWLER_LEVEL_PARSER_HPP
#define DUNGEON_CRAWLER_LEVEL_PARSER_HPP
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <ecs/types.hpp>
#include <cstdlib>
#include <cmath>

using namespace types;

class LevelParserException : public std::exception {
    const std::string _message;;

public:
    explicit LevelParserException(const std::string message) : _message(message) {}
    explicit LevelParserException() = delete;
    explicit LevelParserException(LevelParserException &&) = delete;
    [[nodiscard]] const char * what() const noexcept override {
        return _message.c_str();
    }
};

namespace level_schema {
    enum class schema_types {
        size_x,
        size_y,
        tiles,
        walls,
        wall,
        door,
        type_opened,
        type_closed,
        state,
        between,
        floor,
        walkable,
        player_spawn,
        x,
        y,
        direction,
        encounter_chance
    };
    static std::unordered_map<schema_types, std::string_view> names = {
            {schema_types::size_x, "size_x"},
            {schema_types::size_y, "size_y"},
            {schema_types::tiles, "tiles"},
            {schema_types::walls, "walls"},
            {schema_types::wall, "wall"},
            {schema_types::door, "door"},
            {schema_types::type_opened, "type_opened"},
            {schema_types::type_closed, "type_closed"},
            {schema_types::state, "state"},
            {schema_types::between, "between"},
            {schema_types::floor, "floor"},
            {schema_types::walkable, "walkable"},
            {schema_types::player_spawn, "player_spawn"},
            {schema_types::x, "x"},
            {schema_types::y, "y"},
            {schema_types::direction, "direction"},
            {schema_types::encounter_chance, "encounter_chance"},
    };
    static std::unordered_map<std::string_view, WorldDirection> name_to_direction = {
            {"NORTH", WorldDirection::NORTH},
            {"EAST", WorldDirection::EAST},
            {"SOUTH", WorldDirection::SOUTH},
            {"WEST", WorldDirection::WEST}
    };
    static std::unordered_map<WorldDirection, std::string_view> direction_to_name = {
            {WorldDirection::NORTH, "NORTH"},
            {WorldDirection::EAST, "EAST"},
            {WorldDirection::SOUTH, "SOUTH"},
            {WorldDirection::WEST, "WEST"}
    };
} // namespace assets

class LevelParser {
public:
    static nlohmann::json parse(const std::string &path);
    static nlohmann::json parse_json(const nlohmann::json &json);
    static void save(const std::string &path, const nlohmann::json &json);
private:
    static void _validate(const nlohmann::json &json);
};
#endif//DUNGEON_CRAWLER_LEVEL_PARSER_HPP
