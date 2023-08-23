//
// Created by Patryk Szczypień on 09/03/2023.
//
#include <level_parser.hpp>

using namespace level_schema;

/**
 * Parses a JSON file at the given path and returns a nlohmann::json object.
 *
 * @param path The path to the JSON file.
 * @return The parsed JSON data.
 */
nlohmann::json LevelParser::parse(const std::string &path) {
    std::ifstream file(path.c_str());
    auto json = nlohmann::json::parse(file);
    _validate(json);
    return json;
}

/**
 * @brief Parses a JSON object
 *
 * This function takes a JSON object as input and validates it before returning it.
 * The validation is performed by the "_validate" function.
 *
 * @param json The JSON object to parse
 * @return The parsed JSON object
 *
 * @see _validate
 */
nlohmann::json LevelParser::parse_json(const nlohmann::json &json) {
    _validate(json);
    return json;
}

/**
 * Saves the given JSON object to a file.
 *
 * @param path The path of the file to save.
 * @param json The JSON object to save.
 */
void LevelParser::save(const std::string &path, const nlohmann::json &json) {
    std::ofstream file(path.c_str());
    file << json.dump(4);
}

/**
 * @brief Validates the size definition of an object
 *
 * This function checks if the given JSON object contains the `size_x` and `size_y` definitions. If any of
 * them is missing, it throws a `LevelParserException`. It also checks if `size_x` and `size_y` are positive
 * integers. If any of them is not, it throws a `LevelParserException`.
 *
 * @param json The JSON object to validate
 *
 * @throws LevelParserException If `size_x` or `size_y` is missing or invalid
 */
static inline void validate_definition_size_xy(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::size_x])) {
        throw LevelParserException("Missing `size_x` definition.");
    } else if (!json.contains(names[schema_types::size_y])) {
        throw LevelParserException("Missing `size_y` definition.");
    }
    int size_x = json[names[schema_types::size_x].data()];
    int size_y = json[names[schema_types::size_y].data()];
    if (size_x < 1) {
        throw LevelParserException("`size_x` must be a value equal or greater than 1.");
    }
    if (size_y < 1) {
        throw LevelParserException("`size_y` must be a value equal or greater than 1.");
    }
}

/**
 * @brief Validate the definition of tiles in the given JSON object.
 *
 * This function checks if the `json` object contains the required `tiles` definition.
 * If it is missing, a `LevelParserException` is thrown.
 *
 * @param json The JSON object to validate.
 *
 * @throws LevelParserException If the `tiles` definition is missing.
 */
static inline void validate_definition_tiles(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::tiles])) {
        throw LevelParserException("Missing `tiles` definition.");
    }
}

/**
 * @brief Validates the `walls` definition in a given JSON object.
 *
 * This function checks if the given JSON object contains the `walls` definition.
 * If the `walls` definition is missing, it throws a LevelParserException.
 *
 * @param json The JSON object to validate.
 * @throw LevelParserException Thrown if `walls` definition is missing.
 */
static inline void validate_definition_walls(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::walls])) {
        throw LevelParserException("Missing `walls` definition.");
    }
}

/**
 * @brief Validates the definition of walls in the given JSON.
 *
 * This function checks if each wall element in the JSON contains the required `wall` definition.
 * If any wall element is missing the definition, a LevelParserException is thrown.
 *
 * @param json The JSON object containing the walls definition.
 *
 * @throws LevelParserException if any wall element is missing the `wall` definition.
 */
static inline void validate_definition_walls_wall(const nlohmann::json &json) {
    size_t index = 0;
    for (const auto &wall: json[names[schema_types::walls].data()]) {
        if (!wall.contains(names[schema_types::wall])) {
            std::ostringstream stringStream;
            stringStream << "Missing `wall` definition in wall element at index " << index << ".";
            throw LevelParserException(stringStream.str());
        }
        index += 1;
    }
}

static inline void validate_definition_walls_door(const nlohmann::json &json) {
    size_t index = 0;
    for (const auto &wall: json[names[schema_types::walls].data()]) {
        if (wall.contains(names[schema_types::door])) {
            auto door = wall[names[schema_types::door].data()];
            if (!door.contains(names[schema_types::type_opened])) {
                std::ostringstream stringStream;
                stringStream << "Missing `type_opened` definition in door element at index " << index << ".";
                throw LevelParserException(stringStream.str());
            }
            if (!door.contains(names[schema_types::type_closed])) {
                std::ostringstream stringStream;
                stringStream << "Missing `type_closed` definition in door element at index " << index << ".";
                throw LevelParserException(stringStream.str());
            }
            if (!door.contains(names[schema_types::state])) {
                std::ostringstream stringStream;
                stringStream << "Missing `state` definition in door element at index " << index << ".";
                throw LevelParserException(stringStream.str());
            }
        }
        index += 1;
    }
}

/**
 * @brief Validates that the 'tiles' array in the provided JSON has the correct size.
 *
 * This function checks if the number of elements in the 'tiles' array is equal to the product
 * of the values of 'size_x' and 'size_y'. If they are not equal, an exception of type
 * LevelParserException is thrown with a descriptive error message.
 *
 * @param json The JSON object to validate.
 *
 * @throws LevelParserException If the number of elements in 'tiles' is not equal to the product
 *                              of 'size_x' and 'size_y'.
 */
static inline void validate_tiles_have_correct_size(const nlohmann::json &json) {
    int size_x = json[names[schema_types::size_x].data()];
    int size_y = json[names[schema_types::size_y].data()];
    size_t tiles_size = json[names[schema_types::tiles].data()].size();
    if (tiles_size != size_x * size_y) {
        std::ostringstream stringStream;
        stringStream << "Wrong size (" << tiles_size << ") of `tiles` array - must have " << size_x * size_y << " elements.";
        throw LevelParserException(stringStream.str());
    }
}

/**
 * @brief Validates the definition of the player spawn in a JSON object.
 *
 * This function checks if the given JSON object contains the required keys
 * for the player spawn definition, and validates the values accordingly.
 * If any validation fails, it throws a LevelParserException with a specific error message.
 *
 * @param json The JSON object containing the player spawn definition
 *
 * @throws LevelParserException if any validation fails
 */
static inline void validate_definition_player_spawn(const nlohmann::json &json) {
    if (json.contains(names[schema_types::player_spawn])) {
        if (!json[names[schema_types::player_spawn].data()].contains(names[schema_types::x])) {
            throw LevelParserException("Missing `x` definition in `player_spawn`.");
        }
        if (!json[names[schema_types::player_spawn].data()].contains(names[schema_types::y])) {
            throw LevelParserException("Missing `y` definition in `player_spawn`.");
        }
        if (!json[names[schema_types::player_spawn].data()].contains(names[schema_types::direction])) {
            throw LevelParserException("Missing `direction` definition in `player_spawn`.");
        }
        int size_x = json[names[schema_types::size_x].data()];
        int size_y = json[names[schema_types::size_y].data()];
        {
            int x = json[names[schema_types::player_spawn].data()][names[schema_types::x].data()];
            if (x < 0 || x >= size_x) {
                std::ostringstream stringStream;
                stringStream << "`x` value [" << x << "] in `player_spawn` is outside allowed bounds [0.." << size_x - 1 << "].";
                throw LevelParserException(stringStream.str());
            }
        }
        {
            int y = json[names[schema_types::player_spawn].data()][names[schema_types::y].data()];
            if (y < 0 || y >= size_y) {
                std::ostringstream stringStream;
                stringStream << "`y` value [" << y << "] in `player_spawn` is outside allowed bounds [0.." << size_y - 1 << "].";
                throw LevelParserException(stringStream.str());
            }
        }
        std::string direction = json[names[schema_types::player_spawn].data()][names[schema_types::direction].data()];
        if (name_to_direction.find(direction) == name_to_direction.end()) {
            std::ostringstream stringStream;
            stringStream << "Wrong value [" << direction << "] defined for `player_spawn.direction` - Must be one of [NORTH, EAST, WEST, SOUTH].";
            throw LevelParserException(stringStream.str());
        }
    }
}

/**
 * @brief Validates the definition of walls between fields in a given JSON object.
 *
 * This function checks if the walls between fields in the JSON object are defined correctly.
 * If any validation error is encountered, a LevelParserException is thrown with an appropriate error message.
 *
 * @param json The JSON object containing the wall definitions.
 */
static inline void validate_definition_walls_between(const nlohmann::json &json) {
    size_t index = 0;
    for (const auto &wall: json[names[schema_types::walls].data()]) {
        if (!wall.contains(names[schema_types::between])) {
            std::ostringstream stringStream;
            stringStream << "Missing `between` definition in wall element at index " << index << ".";
            throw LevelParserException(stringStream.str());
        } else {
            auto between = wall[names[schema_types::between].data()];
            if (between.size() != 4) {
                std::ostringstream stringStream;
                stringStream << "Wrong size of `between` array in wall element at index " << index << ". Must have 4 integers.";
                throw LevelParserException(stringStream.str());
            } else {
                int size_x = json[names[schema_types::size_x].data()];
                int size_y = json[names[schema_types::size_y].data()];
                if (between[0] < 0 || between[0] >= size_x) {
                    std::ostringstream stringStream;
                    stringStream << "x0 value [" << between[0] << "] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x - 1 << "].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[1] < 0 || between[1] >= size_y) {
                    std::ostringstream stringStream;
                    stringStream << "y0 value [" << between[1] << "] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x - 1 << "].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[2] < 0 || between[2] >= size_x) {
                    std::ostringstream stringStream;
                    stringStream << "x1 value [" << between[2] << "] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x - 1 << "].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[3] < 0 || between[3] >= size_y) {
                    std::ostringstream stringStream;
                    stringStream << "y1 value [" << between[3] << "] out of bounds in `between` array in wall element at index " << index << ". Value must be in range [0.." << size_x - 1 << "].";
                    throw LevelParserException(stringStream.str());
                }
                if (between[0] == between[2] && between[1] == between[3]) {
                    std::ostringstream stringStream;
                    stringStream << "Wall [" << between[0] << ", " << between[1] << ", " << between[2] << ", " << between[3] << "] declared at index " << index << " must be defined between two different fields.";
                    throw LevelParserException(stringStream.str());
                }
                if (between[1] == between[3] && std::abs(static_cast<int>(between[0]) - static_cast<int>(between[2])) != 1) {
                    std::ostringstream stringStream;
                    stringStream << "Wall [" << between[0] << ", " << between[1] << ", " << between[2] << ", " << between[3] << "] declared at index " << index << " is not between two horizontally adjacent fields.";
                    throw LevelParserException(stringStream.str());
                } else if (between[0] == between[2] && std::abs(static_cast<int>(between[1]) - static_cast<int>(between[3])) != 1) {
                    std::ostringstream stringStream;
                    stringStream << "Wall [" << between[0] << ", " << between[1] << ", " << between[2] << ", " << between[3] << "] declared at index " << index << " is not between two vertically adjacent fields.";
                    throw LevelParserException(stringStream.str());
                }
            }
        }
        index += 1;
    }
}

/**
 * @brief Validates the given JSON object for LevelParser.
 *
 * This function calls various validation functions to ensure the correctness of the JSON data.
 * If any validation fails, an exception will be thrown.
 *
 * @param json The JSON object to validate.
 */
void LevelParser::_validate(const nlohmann::json &json) {
    validate_definition_size_xy(json);
    validate_definition_tiles(json);
    validate_tiles_have_correct_size(json);
    validate_definition_walls(json);
    validate_definition_walls_wall(json);
    validate_definition_walls_between(json);
    validate_definition_walls_door(json);
    validate_definition_player_spawn(json);
}
