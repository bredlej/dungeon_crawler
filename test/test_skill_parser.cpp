//
// Created by Patryk Szczypień on 21/08/2023.
//
#include <engine/skill_parser.hpp>
#include <gtest/gtest.h>
#include <skills.hpp>
#include <tuple>

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

TEST_F(SkillParserTest, Test_Skill_Parser_Parses_Defense_Skill) {
    SkillParser skillParser;
    json json = SkillParser::parse_json(R"(
{
  "defense" : [
    {
      "name": "Heal",
      "body_required": "HEAD",
      "target_type": "ALLY",
      "targets": [{
        "type": "SINGLE",
        "chance": 100
      },{
        "type": "NEIGHBORS",
        "chance": 50
      }],
      "hp": 10,
      "sp": 0,
      "roles": ["MAGE"]
    }
  ]
}
    )"_json);
    ASSERT_EQ(json["defense"][0]["name"], "Heal");
}


TEST_F(SkillParserTest, CreatesSkillsMap) {
    skills::SkillsMap::OffensiveSkillMap offensive_skill_map;
    offensive_skill_map.emplace("Fireball", std::make_tuple(
                                                    components::general::Name{"Fireball"},
                                                    types::battle::BodyPart::HEAD,
                                                    types::battle::TargetType::ENEMY,
                                                    skills::SkillsMap::Targets{
                                                            {types::battle::Target::SINGLE, 100},
                                                            {types::battle::Target::NEIGHBOURS, 50}},
                                                    skills::SkillsMap::Damage{
                                                            {types::battle::AttackType::FIRE, types::character::Attribute::MAGIC_ATTACK, 10}},
                                                    skills::SkillsMap::Ailments{
                                                            {types::battle::Ailment::BURN, 30, 5, 3}},
                                                    components::general::SkillCost{10, 0},
                                                    skills::SkillsMap::RoleConstraints{types::character::Role::MAGE},
                                                    skills::SkillsMap::Followups{
                                                            {{types::battle::AttackType::FIRE},
                                                             3,
                                                             1,
                                                             100,
                                                             50,
                                                             skills::SkillsMap::Damage{
                                                                     {types::battle::AttackType::FIRE, types::character::Attribute::MAGIC_ATTACK, 10.0f}},
                                                             skills::SkillsMap::Ailments{}}}));
    skills::SkillsMap skills(std::move(offensive_skill_map));

    ASSERT_EQ(std::get<components::general::Name>(skills.offensive_skills["Fireball"]).name, "Fireball");
    ASSERT_EQ(std::get<types::battle::BodyPart>(skills.offensive_skills["Fireball"]), types::battle::BodyPart::HEAD);
    ASSERT_EQ(std::get<types::battle::TargetType>(skills.offensive_skills["Fireball"]), types::battle::TargetType::ENEMY);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"]).size(), 2);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[0].first, types::battle::Target::SINGLE);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[0].second, 100);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[1].first, types::battle::Target::NEIGHBOURS);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[1].second, 50);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"])[0].type, types::battle::AttackType::FIRE);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"])[0].attribute, types::character::Attribute::MAGIC_ATTACK);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"])[0].damage_value, 10);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].type, types::battle::Ailment::BURN);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].chance, 30);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].damage_value, 5);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].duration, 3);
    ASSERT_EQ(std::get<components::general::SkillCost>(skills.offensive_skills["Fireball"]).sp, 10);
    ASSERT_EQ(std::get<components::general::SkillCost>(skills.offensive_skills["Fireball"]).hp, 0);
    ASSERT_EQ(std::get<skills::SkillsMap::RoleConstraints>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::RoleConstraints>(skills.offensive_skills["Fireball"])[0], types::character::Role::MAGE);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].on_damage_type[0], types::battle::AttackType::FIRE);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].duration, 3);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].max_stack, 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].initial_chance, 100);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].damage_reduction_percent, 50);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].damage.size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].damage[0].type, types::battle::AttackType::FIRE);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].damage[0].attribute, types::character::Attribute::MAGIC_ATTACK);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].damage[0].damage_value, 10);
    ASSERT_EQ(std::get<skills::SkillsMap::Followups>(skills.offensive_skills["Fireball"])[0].ailments.size(), 0);
}

TEST_F(SkillParserTest, SkillMapFromJson) {
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
        "type": "NEIGHBOURS",
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

    skills::SkillsMap skills = skills::from_json(json);

    ASSERT_EQ(std::get<components::general::Name>(skills.offensive_skills["Fireball"]).name, "Fireball");
    ASSERT_EQ(std::get<types::battle::BodyPart>(skills.offensive_skills["Fireball"]), types::battle::BodyPart::HEAD);
    ASSERT_EQ(std::get<types::battle::TargetType>(skills.offensive_skills["Fireball"]), types::battle::TargetType::ENEMY);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"]).size(), 2);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[0].first, types::battle::Target::SINGLE);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[0].second, 100);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[1].first, types::battle::Target::NEIGHBOURS);
    ASSERT_EQ(std::get<skills::SkillsMap::Targets>(skills.offensive_skills["Fireball"])[1].second, 50);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"])[0].type, types::battle::AttackType::FIRE);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"])[0].attribute, types::character::Attribute::MAGIC_ATTACK);
    ASSERT_EQ(std::get<skills::SkillsMap::Damage>(skills.offensive_skills["Fireball"])[0].damage_value, 10);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].type, types::battle::Ailment::BURN);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].chance, 30);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].damage_value, 5);
    ASSERT_EQ(std::get<skills::SkillsMap::Ailments>(skills.offensive_skills["Fireball"])[0].duration, 3);
    ASSERT_EQ(std::get<components::general::SkillCost>(skills.offensive_skills["Fireball"]).sp, 10);
    ASSERT_EQ(std::get<components::general::SkillCost>(skills.offensive_skills["Fireball"]).hp, 0);
    ASSERT_EQ(std::get<skills::SkillsMap::RoleConstraints>(skills.offensive_skills["Fireball"]).size(), 1);
    ASSERT_EQ(std::get<skills::SkillsMap::RoleConstraints>(skills.offensive_skills["Fireball"])[0], types::character::Role::MAGE);
}

TEST_F(SkillParserTest, JsonToBodyPartEnum) {
    using namespace skill_schema;
    nlohmann::json json = R"(
        {
          "body_required": "HEAD"
        }
    )"_json;
    std::string value = json[names[schema_types::body_required].data()];
    types::battle::BodyPart body_part{types::battle::string_to_body_part[value]};

    ASSERT_EQ(body_part, types::battle::BodyPart::HEAD);
    ASSERT_EQ(types::battle::string_to_body_part["ARMS"], types::battle::BodyPart::ARMS);
    ASSERT_EQ(types::battle::string_to_body_part["LEGS"], types::battle::BodyPart::LEGS);
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