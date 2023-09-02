//
// Created by Patryk Szczypień on 17/08/2023.
//
#include <core.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <typeindex>
#include <components.hpp>
#include <events.hpp>
#include <combat.hpp>
#include <nlohmann/json.hpp>
#include <skill_parser.hpp>
#include <skills.hpp>

constexpr auto TEST_SEED = 1234123123;

class CombatTest : public testing::Test {
protected:
    CombatTest() = default;
    nlohmann::json json = SkillParser::parse_json(R"(
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
    },
    {
        "name": "Ice dance",
        "body_required": "LEGS",
        "target_type": "ENEMY",
        "targets": [
        {
          "type": "SINGLE",
          "chance": 100
        }
        ],
        "damage": [
        {
          "type": "ICE",
          "attribute": "MAGIC_ATTACK",
          "damage_value": 10
        },
        {
          "type": "PIERCE",
            "attribute": "ATTACK",
            "damage_value": 5
        }
        ],
        "ailments": [],
        "sp": 10,
        "hp": 0,
        "roles": [
          "BLADE_DANCER"
        ],
        "followups": [{
        "on_damage_type": ["ICE", "PIERCE"],
        "duration": 1,
        "max_stack": 5,
        "initial_chance": 100,
        "damage_reduction_percent": 10,
        "damage": [
            {
                "type": "ICE",
                "attribute": "MAGIC_ATTACK",
                "damage_value": 10
            },
            {
                "type": "PIERCE",
                "attribute": "ATTACK",
                "damage_value": 5
            }
        ],
        "ailments": []
        }]
        }

  ]
}
    )"_json);
    skills::SkillsMap skills_map = skills::from_json(json);

};

static Attributes create_attributes(const std::shared_ptr<Core> &core, entt::entity entity) {
    auto &attributes = core->registry.emplace<Attributes>(entity);
    return attributes;
}

static void add_attribute(const std::shared_ptr<Core> &core, entt::entity entity, character::Attribute attribute, float value) {
    auto &attributes = core->registry.get<Attributes>(entity);
    attributes.attributes[attribute] = value;
}

static entt::entity create_character(const std::shared_ptr<Core> &core, const std::string &name, float hit_points) {
    auto character = core->registry.create();
    create_attributes(core, character);
    add_attribute(core, character, character::Attribute::HIT_POINTS, hit_points);
    core->registry.emplace<components::general::Name>(character, name);
    return character;
}

static void cast_fireball(const std::shared_ptr<Core> &core, Combat &combat, auto &skills_map, auto attacker, auto target, auto neighbours) {
    auto fireball_func = skills::instance_offensive_skill(core, skills_map, "Fireball");
    auto fireball = fireball_func(core);
    core->registry.emplace_or_replace<targets::TargetSingle>(fireball, target, 1.0f);
    core->registry.emplace_or_replace<targets::TargetMultiple>(fireball, neighbours, 0.5f);

    combat._combat_dispatcher.trigger(events::battle::AttackEvent{attacker, fireball});
}

static void cast_ice_dance(const std::shared_ptr<Core> &core, Combat &combat, auto &skills_map, auto attacker, auto target) {
    auto ice_dance_func = skills::instance_offensive_skill(core, skills_map, "Ice dance");
    auto ice_dance = ice_dance_func(core);
    core->registry.emplace_or_replace<targets::TargetSingle>(ice_dance, target, 1.0f);

    combat._combat_dispatcher.trigger(events::battle::AttackEvent{attacker, ice_dance});
}

TEST_F(CombatTest, Fireball) {

    constexpr auto GHOUL_HIT_POINTS = 20.0f;
    std::shared_ptr<Core> core = std::make_shared<Core>();
    core->pcg.seed(TEST_SEED);
    Combat combat{core};

    auto attacker = create_character(core, "Sorcerer", 100.0f);
    auto target = create_character(core, "Ghoul #2", GHOUL_HIT_POINTS);
    core->registry.emplace<components::battle::mods::Weakness<components::battle::damage_types::FireDmg>>(target, 1.5f);
    auto neighbours = std::vector<entt::entity>{create_character(core, "Ghoul #1", GHOUL_HIT_POINTS), create_character(core, "Ghoul #3", GHOUL_HIT_POINTS)};
    core->registry.emplace<components::battle::mods::Immunity<components::battle::damage_types::FireDmg>>(neighbours[0], 0.0f);
    core->registry.emplace<components::battle::mods::Resistance<components::battle::damage_types::FireDmg>>(neighbours[1], 0.5f);
    core->registry.emplace<Neighbours>(target, neighbours);

    for (int i =0; i < 5; i++) {
        cast_fireball(core, combat, skills_map, attacker, target, neighbours);
    }
    ASSERT_FALSE(core->try_get<ailments::Death>(neighbours[1])); // resistant to fire

    cast_fireball(core, combat, skills_map, attacker, target, neighbours);
    ASSERT_TRUE(core->try_get<ailments::Death>(target)); // weak to fire
    ASSERT_FALSE(core->try_get<ailments::Death>(neighbours[0])); // immune to fire
    ASSERT_TRUE(core->try_get<ailments::Death>(neighbours[1])); // resistant to fire
}

TEST_F(CombatTest, IceDance) {
    constexpr auto GHOUL_HIT_POINTS = 20.0f;
    std::shared_ptr<Core> core = std::make_shared<Core>();
    core->pcg.seed(TEST_SEED);
    Combat combat{core};

    auto attacker = create_character(core, "Sorcerer", 100.0f);
    auto target = create_character(core, "Ghoul #2", GHOUL_HIT_POINTS);
    auto neighbours = std::vector<entt::entity>{create_character(core, "Ghoul #1", GHOUL_HIT_POINTS), create_character(core, "Ghoul #3", GHOUL_HIT_POINTS)};
    core->registry.emplace<Neighbours>(target, neighbours);

    cast_ice_dance(core, combat, skills_map, attacker, target);
    ASSERT_TRUE(core->try_get<ailments::Death>(target));
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
