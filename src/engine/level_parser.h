//
// Created by Patryk Szczypień on 09/03/2023.
//

#ifndef DUNGEON_CRAWLER_LEVEL_PARSER_H
#define DUNGEON_CRAWLER_LEVEL_PARSER_H
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

namespace level_schema {
    enum class types {
        size_x,
        size_y,
        tiles,
        walls
    };
    static std::unordered_map<types, const char *> names = {
            {types::size_x, "size_x"},
            {types::size_y, "size_y"},
            {types::tiles, "tiles"},
            {types::walls, "walls"}
    };
} // namespace assets

class LevelParser {
public:
    nlohmann::json parse(const std::string &path);
};
#endif//DUNGEON_CRAWLER_LEVEL_PARSER_H
