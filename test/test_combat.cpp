//
// Created by Patryk Szczypień on 17/08/2023.
//
#include "engine/battle_system/combat.hpp"
#include <components.hpp>
#include <core.hpp>
#include <engine/parsers/skill_parser.hpp>
#include <events.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <parsers/monster_parser.hpp>
#include <random>
#include <skills.hpp>
#include <monsters.hpp>
#include <typeindex>

constexpr auto TEST_SEED = 1234123123;

class CombatTest : public testing::Test {
protected:
    CombatTest() = default;
    nlohmann::json skills_json = SkillParser::parse("resources/test_skills.json");
    nlohmann::json monsters_json = MonsterParser::parse("resources/test_monsters.json", AttributesParser{}, skills::SkillsMap::from_json(skills_json));
    skills::SkillsMap skills_map = skills::SkillsMap::from_json(skills_json);
    MonstersMap monsters_map = MonstersMap::from_json(monsters_json);
};

static Attributes create_attributes(const std::shared_ptr<Core> &core, entt::entity entity) {
    auto &attributes = core->registry.emplace<Attributes>(entity);
    return attributes;
}

static void add_attribute(const std::shared_ptr<Core> &core, const entt::entity entity, const character::Attribute attribute, const float value) {
    auto &[attributes] = core->registry.get<Attributes>(entity);
    attributes[attribute] = value;
}

static entt::entity create_character(const std::shared_ptr<Core> &core, const std::string &name, const float hit_points) {
    const auto character = core->registry.create();
    create_attributes(core, character);
    add_attribute(core, character, character::Attribute::HIT_POINTS, hit_points);
    core->registry.emplace<components::general::Name>(character, name);
    std::printf("Created character %s with %.0f hit points (id=%u)\n", name.data(), hit_points, character);
    return character;
}

static void cast_fireball(const std::shared_ptr<Core> &core, Combat &combat, auto &skills_map, auto attacker, auto target, auto neighbours) {
    std::printf("Casting fireball on target with id=%u\n", target);
    auto fireball_func = skills::SkillsMap::instance_offensive_skill(core, skills_map, "FIREBALL");
    auto fireball = fireball_func(core);
    core->registry.emplace_or_replace<targets::TargetSingle>(fireball, target, 1.0f);
    core->registry.emplace_or_replace<targets::TargetMultiple>(fireball, neighbours, 0.5f);

    combat._combat_dispatcher.trigger(events::battle::AttackEvent{attacker, fireball});
}

static void cast_ice_bolt(const std::shared_ptr<Core> &core, Combat &combat, auto &skills_map, auto attacker, auto target) {
    auto ice_bolt_func = skills::SkillsMap::instance_offensive_skill(core, skills_map, "ICE_BOLT");
    auto ice_bolt = ice_bolt_func(core);
    core->registry.emplace_or_replace<targets::TargetSingle>(ice_bolt, target, 1.0f);

    combat._combat_dispatcher.trigger(events::battle::AttackEvent{attacker, ice_bolt});
}
static void cast_ice_dance(const std::shared_ptr<Core> &core, Combat &combat, auto &skills_map, auto attacker, auto target) {
    auto ice_dance_func = skills::SkillsMap::instance_offensive_skill(core, skills_map, "ICE_DANCE");
    auto ice_dance = ice_dance_func(core);
    core->registry.emplace_or_replace<targets::TargetSingle>(ice_dance, target, 1.0f);

    combat._combat_dispatcher.trigger(events::battle::AttackEvent{attacker, ice_dance});
}

TEST_F(CombatTest, Fireball) {

    std::printf("\n\t--------------------\n");
    std::printf("\tTesting fireball\n");
    std::printf("\t--------------------\n");
    constexpr auto GHOUL_HIT_POINTS = 20.0f;
    const auto core = std::make_shared<Core>();
    core->pcg.seed(TEST_SEED);
    Combat combat{core};

    const auto attacker = create_character(core, "Sorcerer", 100.0f);
    const auto target = create_character(core, "Ghoul #2", GHOUL_HIT_POINTS);
    core->registry.emplace<mods::Weakness<damage_types::FireDmg>>(target, 1.5f);

    auto neighbours = std::vector{create_character(core, "Ghoul #1", GHOUL_HIT_POINTS), create_character(core, "Ghoul #3", GHOUL_HIT_POINTS)};
    core->registry.emplace<mods::Immunity<damage_types::FireDmg>>(neighbours[0], 0.0f);
    core->registry.emplace<mods::Resistance<damage_types::FireDmg>>(neighbours[1], 0.5f);
    core->registry.emplace<Neighbours>(target, neighbours);

    for (int i = 0; i < 5; i++) {
        cast_fireball(core, combat, skills_map, attacker, target, neighbours);
    }
    ASSERT_FALSE(core->try_get<ailments::Death>(neighbours[1]));// resistant to fire

    cast_fireball(core, combat, skills_map, attacker, target, neighbours);
    ASSERT_TRUE(core->try_get<ailments::Death>(target));        // weak to fire
    ASSERT_TRUE(core->try_get<ailments::Death>(neighbours[1])); // resistant to fire
    ASSERT_FALSE(core->try_get<ailments::Death>(neighbours[0]));// immune to fire
}

TEST_F(CombatTest, IceDance) {
    std::printf("\n\t--------------------\n");
    std::printf("\tTesting ice dance\n");
    std::printf("\t--------------------\n");
    constexpr auto KOBOLD_HIT_POINTS = 10.0f;
    const auto core = std::make_shared<Core>();
    core->pcg.seed(TEST_SEED);
    Combat combat{core};

    const auto attacker = create_character(core, "Blade dancer", 100.0f);
    const auto second_attacker = create_character(core, "Sorcerer", 100.0f);
    const auto target = create_character(core, "Kobold (immune to ice)", KOBOLD_HIT_POINTS);
    core->registry.emplace<mods::Immunity<damage_types::IceDmg>>(target, 0.0f);
    cast_ice_dance(core, combat, skills_map, attacker, target);
    cast_ice_bolt(core, combat, skills_map, second_attacker, target);
    ASSERT_TRUE(core->try_get<ailments::Death>(target));
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
