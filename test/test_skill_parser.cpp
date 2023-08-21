//
// Created by Patryk Szczypień on 21/08/2023.
//
#include <engine/skill_parser.hpp>
#include <gtest/gtest.h>

using namespace nlohmann;

class SkillParserTest : public testing::Test {
protected:
    SkillParserTest() = default;
};

TEST_F(SkillParserTest, Test_Skill_Parser_Initializes) {
    SkillParser skillParser;
}

TEST_F(SkillParserTest, Test_Skill_Parser_Parses_Offense_Skill) {
    SkillParser skillParser;
    json json = SkillParser::parse_json(R"(
{
  "offense" : [
    {
      "name": "Fireball",
      "body_required": "HEAD",
      "target_type": "ENEMY",
      "targets": [{
        "type": "SINGLE",
        "chance": 100
      },{
        "type": "NEIGHBORS",
        "chance": 50
      }],
      "damage": [{
        "type": "FIRE",
        "attribute": "MAGIC_ATTACK",
        "damage_value": 10
      }],
      "ailments" : [
        {
            "type": "BURN",
            "chance": 30,
            "damage_value": 5,
            "duration": 3
        }
      ],
      "sp": 10,
      "hp": 0,
      "roles": ["MAGE"]
    }
  ]
}
    )"_json);
    ASSERT_EQ(json["offense"][0]["name"], "Fireball");
}

TEST_F(SkillParserTest, Test_Skill_Parser_Throws_Exception_When_Skill_Does_Not_Have_Name) {
    SkillParser skillParser;
    ASSERT_THROW(skillParser.parse_json(R"(
        {
            "offense" : [
                {
                    "body_required": 1,
                    "target_type": "single",
                    "targets": [
                        {
                            "type": "damage",
                            "chance": 100,
                            "damage": {
                                "value": 10
                            }
                        }
                    ]
                }
            ]
        }
    )"_json),
                 SkillParserException);
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}