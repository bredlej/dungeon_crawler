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

nlohmann::json LevelParser::parse_json(const nlohmann::json &json) {
    _validate(json);
    return json;
}

static inline void validate_definition_size_xy(const nlohmann::json &json) {
    if (!json.contains(names[types::size_x])) {
        throw LevelParserException("Missing `size_x` definition.");
    }
    else if (!json.contains(names[types::size_y])) {
        throw LevelParserException("Missing `size_y` definition.");
    }
    int size_x = json[names[types::size_x]];
    int size_y = json[names[types::size_y]];
    if (size_x < 1 ) {
        throw LevelParserException("`size_x` must be a value equal or greater than 1.");
    }
    if (size_y < 1 ) {
        throw LevelParserException("`size_y` must be a value equal or greater than 1.");
    }
}

static inline void validate_definition_tiles(const nlohmann::json &json) {
    if (!json.contains(names[types::tiles])) {
        throw LevelParserException("Missing `tiles` definition.");
    }
}

static inline void validate_definition_walls(const nlohmann::json &json) {
    if (!json.contains(names[types::walls])) {
        throw LevelParserException("Missing `walls` definition.");
    }
}

static inline void validate_definition_walls_wall(const nlohmann::json &json) {
    size_t index = 0;
    for (const auto& wall: json[names[types::walls]]) {
        if (!wall.contains(names[types::wall])) {
            std::ostringstream stringStream;
            stringStream << "Missing `wall` definition in wall element at index " << index << ".";
            throw LevelParserException(stringStream.str());
        }
        index += 1;
    }
}

static inline void validate_tiles_have_correct_size(const nlohmann::json &json) {
    int size_x = json[names[types::size_x]];
    int size_y = json[names[types::size_y]];
    size_t tiles_size = json[names[types::tiles]].size();
    if (tiles_size != size_x * size_y) {
        std::ostringstream stringStream;
        stringStream << "Wrong size (" << tiles_size << ") of `tiles` array - must have " << size_x * size_y << " elements.";
        throw LevelParserException(stringStream.str());
    }
}
static inline void validate_definition_player_spawn(const nlohmann::json &json) {
    if (json.contains(names[types::player_spawn])) {
        if (!json[names[types::player_spawn]].contains(names[types::x])) {
            throw LevelParserException("Missing `x` definition in `player_spawn`.");
        }
        if (!json[names[types::player_spawn]].contains(names[types::y])) {
            throw LevelParserException("Missing `y` definition in `player_spawn`.");
        }
        if (!json[names[types::player_spawn]].contains(names[types::direction])) {
            throw LevelParserException("Missing `direction` definition in `player_spawn`.");
        }
        int size_x = json[names[types::size_x]];
        int size_y = json[names[types::size_y]];
        {
            int x = json[names[types::player_spawn]][names[types::x]];
            if (x < 0 || x >= size_x) {
                std::ostringstream stringStream;
                stringStream << "`x` value ["<< x <<"] in `player_spawn` is outside allowed bounds [0.." << size_x -1 << "].";
                throw LevelParserException(stringStream.str());
            }
        }
        {
            int y = json[names[types::player_spawn]][names[types::y]];
            if (y < 0 || y >= size_y) {
                std::ostringstream stringStream;
                stringStream << "`y` value ["<< y <<"] in `player_spawn` is outside allowed bounds [0.." << size_y -1 << "].";
                throw LevelParserException(stringStream.str());
            }
        }
        std::string direction = json[names[types::player_spawn]][names[types::direction]];
        if (direction_names.find(direction) == direction_names.end()) {
            std::ostringstream stringStream;
            stringStream << "Wrong value [" << direction << "] defined for `player_spawn.direction` - Must be one of [NORTH, EAST, WEST, SOUTH].";
            throw LevelParserException(stringStream.str());
        }
    }
}

static inline void validate_definition_walls_between(const nlohmann::json &json) {
    size_t index = 0;
    for (const auto &wall: json[names[types::walls]]) {
        if (!wall.contains(names[types::between])) {
            std::ostringstream stringStream;
            stringStream << "Missing `between` definition in wall element at index " << index << ".";
            throw LevelParserException(stringStream.str());
        } else {
            auto between = wall[names[types::between]];
            if (between.size() != 4) {
                std::ostringstream stringStream;
                stringStream << "Wrong size of `between` array in wall element at index " << index << ". Must have 4 integers.";
                throw LevelParserException(stringStream.str());
            }
            else {
                int size_x = json[names[types::size_x]];
                int size_y = json[names[types::size_y]];
                if (between[0] < 0 || between[0] >= size_x) {
                    std::ostringstream stringStream;
                    stringStream << "x0 value ["<<between[0]<<"] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x-1 <<"].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[1] < 0 || between[1] >= size_y) {
                    std::ostringstream stringStream;
                    stringStream << "y0 value ["<<between[1]<<"] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x-1 <<"].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[2] < 0 || between[2] >= size_x) {
                    std::ostringstream stringStream;
                    stringStream << "x1 value ["<<between[2]<<"] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x-1 <<"].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[3] < 0 || between[3] >= size_y) {
                    std::ostringstream stringStream;
                    stringStream << "y1 value ["<<between[3]<<"] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x-1 <<"].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[0] == between[2] && between[1] == between[3]) {
                    std::ostringstream stringStream;
                    stringStream << "Wall ["<< between[0] <<", "<< between[1] <<", "<< between[2] <<", "<< between[3] <<"] declared at index " << index << " must be defined between two different fields.";
                    throw LevelParserException(stringStream.str());
                }
                if (between[1] == between[3] && std::abs(static_cast<int>(between[0]) - static_cast<int>(between[2])) != 1) {
                    std::ostringstream stringStream;
                    stringStream << "Wall ["<< between[0] <<", "<< between[1] <<", "<< between[2] <<", "<< between[3] <<"] declared at index " << index << " is not between two horizontally adjacent fields.";
                    throw LevelParserException(stringStream.str());
                }
                else if (between[0] == between[2] && std::abs(static_cast<int>(between[1]) - static_cast<int>(between[3])) != 1) {
                    std::ostringstream stringStream;
                    stringStream << "Wall ["<< between[0] <<", "<< between[1] <<", "<< between[2] <<", "<< between[3] <<"] declared at index " << index << " is not between two vertically adjacent fields.";
                    throw LevelParserException(stringStream.str());
                }
            }
        }
        index += 1;
    }
}

void LevelParser::_validate(const nlohmann::json &json) {
    validate_definition_size_xy(json);
    validate_definition_tiles(json);
    validate_tiles_have_correct_size(json);
    validate_definition_walls(json);
    validate_definition_walls_wall(json);
    validate_definition_walls_between(json);
    validate_definition_player_spawn(json);
}
