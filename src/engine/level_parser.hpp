//
// Created by Patryk Szczypień on 09/03/2023.
//

#ifndef DUNGEON_CRAWLER_LEVEL_PARSER_HPP
#define DUNGEON_CRAWLER_LEVEL_PARSER_HPP
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <format>
#include <ecs/types.hpp>
#include <cstdlib>
#include <cmath>

class LevelParserException : public std::exception {
    const std::string _message;;

public:
    explicit LevelParserException(const std::string message) : _message(message) {}
    explicit LevelParserException() = delete;
    explicit LevelParserException(LevelParserException &&) = delete;
    const char * what() const noexcept override {
        return _message.c_str();
    }
};

namespace level_schema {
    enum class types {
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
    static std::unordered_map<types, const char *> names = {
            {types::size_x, "size_x"},
            {types::size_y, "size_y"},
            {types::tiles, "tiles"},
            {types::walls, "walls"},
            {types::wall, "wall"},
            {types::door, "door"},
            {types::type_opened, "type_opened"},
            {types::type_closed, "type_closed"},
            {types::state, "state"},
            {types::between, "between"},
            {types::floor, "floor"},
            {types::walkable, "walkable"},
            {types::player_spawn, "player_spawn"},
            {types::x, "x"},
            {types::y, "y"},
            {types::direction, "direction"},
            {types::encounter_chance, "encounter_chance"},
    };
    static std::unordered_map<std::string, WorldDirection> name_to_direction = {
            {"NORTH", WorldDirection::NORTH},
            {"EAST", WorldDirection::EAST},
            {"SOUTH", WorldDirection::SOUTH},
            {"WEST", WorldDirection::WEST}
    };
    static std::unordered_map<WorldDirection, std::string> direction_to_name = {
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
