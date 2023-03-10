//
// Created by Patryk Szczypień on 09/03/2023.
//
#include <level_parser.h>

using namespace level_schema;
nlohmann::json LevelParser::parse(const std::string &path) {
    std::ifstream file(path.c_str());
    auto json = nlohmann::json::parse(file);
    _validate(json);
    return json;
}

void LevelParser::_validate(nlohmann::json &json) {
    if (!json.contains(names[types::size_x])) {
        throw LevelParserException("Missing `size_x` definition.");
    }
    else if (!json.contains(names[types::size_y])) {
        throw LevelParserException("Missing `size_y` definition.");
    }
    else if (!json.contains(names[types::tiles])) {
        throw LevelParserException("Missing `tiles` definition.");
    }
    else if (!json.contains(names[types::walls])) {
        throw LevelParserException("Missing `walls` definition.");
    }
    int size_x = json[names[types::size_x]];
    if (size_x < 1 ) {
        throw LevelParserException("`size_x` must be a value greater than 1.");
    }
    int size_y = json[names[types::size_y]];
    if (size_y < 1 ) {
        throw LevelParserException("`size_y` must be a value greater than 1.");
    }
    long tiles_size = json[names[types::tiles]].size();
    if (tiles_size != size_x * size_y) {
        std::ostringstream stringStream;
        stringStream << "Wrong size (" << tiles_size << ") of `tiles` array - must have " << size_x * size_y << " elements.";
        throw LevelParserException(stringStream.str());
    }
}
