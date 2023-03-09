//
// Created by Patryk Szczypień on 09/03/2023.
//
#include <level_parser.h>

nlohmann::json LevelParser::parse(const std::string &path) {
    std::ifstream file(path.c_str());
    return nlohmann::json::parse(file);
}
