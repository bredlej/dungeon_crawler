//
// Created by Patryk Szczypień on 09/03/2023.
//

#ifndef DUNGEON_CRAWLER_LEVEL_PARSER_H
#define DUNGEON_CRAWLER_LEVEL_PARSER_H
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <format>

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
        floor,
        walkable
    };
    static std::unordered_map<types, const char *> names = {
            {types::size_x, "size_x"},
            {types::size_y, "size_y"},
            {types::tiles, "tiles"},
            {types::walls, "walls"},
            {types::floor, "floor"},
            {types::walkable, "walkable"}
    };
} // namespace assets

class LevelParser {
public:
    nlohmann::json parse(const std::string &path);
private:
    static void _validate(nlohmann::json &json);
};
#endif//DUNGEON_CRAWLER_LEVEL_PARSER_H
