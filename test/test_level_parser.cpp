//
// Created by Patryk Szczypień on 13/03/2023.
//
#include <engine/level_parser.hpp>
#include <gtest/gtest.h>

using namespace nlohmann;

class LevelParserTest : public testing::Test {
protected:
    LevelParserTest() = default;
};

TEST_F(LevelParserTest, Test_SizeX_NotDefined) {
    json json = json::parse(R"(
        {
            "size_y" : 10,
            "tiles" : [],
            "walls" : [],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `size_x` definition.", e.what());
    }
}
TEST_F(LevelParserTest, Test_SizeX_SmallerThanOne) {
    json json = json::parse(R"(
        {
            "size_x" : 0,
            "size_y" : 10,
            "tiles" : [],
            "walls" : [],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("`size_x` must be a value equal or greater than 1.", e.what());
    }
}
TEST_F(LevelParserTest, Test_SizeY_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 10,
            "tiles" : [],
            "walls" : [],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `size_y` definition.", e.what());
    }
}
TEST_F(LevelParserTest, Test_SizeY_SmallerThanOne) {
    json json = json::parse(R"(
        {
            "size_x" : 10,
            "size_y" : 0,
            "tiles" : [],
            "walls" : [],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("`size_y` must be a value equal or greater than 1.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Tiles_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 10,
            "size_y" : 10,
            "walls" : [],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `tiles` definition.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Tiles_WrongSize) {
    json json = json::parse(R"(
        {
            "size_x" : 10,
            "size_y" : 10,
            "tiles" : [[]],
            "walls" : [],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Wrong size (1) of `tiles` array - must have 100 elements.", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_X_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"y" : 1, "direction" : "NORTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `x` definition in `player_spawn`.", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_X_SmallerThanZero) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x": -1, "y" : 1, "direction" : "NORTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("`x` value [-1] in `player_spawn` is outside allowed bounds [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_X_BiggerThanSizeX) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x": 2, "y" : 1, "direction" : "NORTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("`x` value [2] in `player_spawn` is outside allowed bounds [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_Y_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x" : 1, "direction" : "NORTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `y` definition in `player_spawn`.", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_Y_SmallerThanZero) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x": 1, "y" : -1, "direction" : "NORTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("`y` value [-1] in `player_spawn` is outside allowed bounds [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_Y_BiggerThanSizeY) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x": 1, "y" : 2, "direction" : "NORTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("`y` value [2] in `player_spawn` is outside allowed bounds [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_Direction_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x" : 1, "y" : 1}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `direction` definition in `player_spawn`.", e.what());
    }
}
TEST_F(LevelParserTest, Test_PlayerSpawn_Direction_InvalidValue) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [],
            "player_spawn" : {"x" : 1, "y" : 1, "direction" : "NORF"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Wrong value [NORF] defined for `player_spawn.direction` - Must be one of [NORTH, EAST, WEST, SOUTH].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `walls` definition.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Wall_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"between" : [0,0,1,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `wall` definition in wall element at index 0.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_NotDefined) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL"}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Missing `between` definition in wall element at index 0.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_WrongSize) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between" : [0,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Wrong size of `between` array in wall element at index 0. Must have 4 integers.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_X0_SmallerThanZero) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [-1,0,1,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("x0 value [-1] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_X1_SmallerThanZero) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,-1,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("x1 value [-1] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_Y0_SmallerThanZero) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,-1,0,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("y0 value [-1] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_Y1_SmallerThanZero) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,0,-1]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("y1 value [-1] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_X0_BiggerThanSizeX) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [2,0,1,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("x0 value [2] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_X1_BiggerThanSizeX) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,2,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("x1 value [2] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_Y0_BiggerThanSizeY) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,2,0,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("y0 value [2] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_Y1_BiggerThanSizeY) {
    json json = json::parse(R"(
        {
            "size_x" : 2,
            "size_y" : 2,
            "tiles" : [[], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,0,2]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("y1 value [2] out of bounds in `between` array in wall element at index 0. Value must be in range [0..1].", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_NotBetweenSameField) {
    json json = json::parse(R"(
        {
            "size_x" : 3,
            "size_y" : 3,
            "tiles" : [[], [], [], [], [], [], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,0,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Wall [0, 0, 0, 0] declared at index 0 must be defined between two different fields.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_NotAdjacentHorizontal) {
    json json = json::parse(R"(
        {
            "size_x" : 3,
            "size_y" : 3,
            "tiles" : [[], [], [], [], [], [], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,2,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Wall [0, 0, 2, 0] declared at index 0 is not between two horizontally adjacent fields.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_AdjacentHorizontal) {
    json json = json::parse(R"(
        {
            "size_x" : 3,
            "size_y" : 3,
            "tiles" : [[], [], [], [], [], [], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,1,0]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        ASSERT_EQ(1,1);
    }
    catch (std::exception &e) {
        std::printf("%s", e.what());
        FAIL();    }
}
TEST_F(LevelParserTest, Test_Walls_Between_NotAdjacentVertical) {
    json json = json::parse(R"(
        {
            "size_x" : 3,
            "size_y" : 3,
            "tiles" : [[], [], [], [], [], [], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,0,2]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        FAIL();
    }
    catch (std::exception &e) {
        ASSERT_STREQ("Wall [0, 0, 0, 2] declared at index 0 is not between two vertically adjacent fields.", e.what());
    }
}
TEST_F(LevelParserTest, Test_Walls_Between_AdjacentVertical) {
    json json = json::parse(R"(
        {
            "size_x" : 3,
            "size_y" : 3,
            "tiles" : [[], [], [], [], [], [], [], [], []],
            "walls" : [{"wall" : "SOME_WALL", "between": [0,0,0,1]}],
            "player_spawn" : {"x": 1, "y": 1, "direction": "SOUTH"}
        }
    )");

    try {
        LevelParser::parse_json(json);
        ASSERT_EQ(1,1);
    }
    catch (std::exception &e) {
        FAIL();    }
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}