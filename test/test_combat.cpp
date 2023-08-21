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

constexpr auto TEST_SEED = 1234123123;

class CombatTest : public testing::Test {
protected:
    CombatTest() = default;
};

TEST_F(CombatTest, Fireball) {

    constexpr auto GHOUL_HIT_POINTS = 20.0f;

    std::shared_ptr<Core> core = std::make_shared<Core>();
    core->pcg.seed(TEST_SEED);
    Combat combat{core};
    auto attacker = combat.create_character("Sorcerer", 100.0f);
    auto target = combat.create_character("Ghoul #2", GHOUL_HIT_POINTS);
    core->registry.emplace<components::battle::mods::Weakness<components::battle::damage_types::FireDmg>>(target, 1.5f);
    auto neighbours = std::vector<entt::entity>{combat.create_character("Ghoul #1", GHOUL_HIT_POINTS), combat.create_character("Ghoul #3", GHOUL_HIT_POINTS)};
    core->registry.emplace<components::battle::mods::Immunity<components::battle::damage_types::FireDmg>>(neighbours[0], 0.0f);
    core->registry.emplace<components::battle::mods::Resistance<components::battle::damage_types::FireDmg>>(neighbours[1], 0.5f);
    core->registry.emplace<Neighbours>(target, neighbours);

    combat.create_fireball(attacker, target, neighbours);
    combat.create_fireball(attacker, target, neighbours);
    combat.create_fireball(attacker, target, neighbours);
    combat.create_fireball(attacker, target, neighbours);
    combat.create_fireball(attacker, target, neighbours);

    ASSERT_FALSE(core->try_get<ailments::Death>(neighbours[1])); // resistant to fire
    combat.create_fireball(attacker, target, neighbours);

    ASSERT_TRUE(core->try_get<ailments::Death>(target)); // weak to fire
    ASSERT_FALSE(core->try_get<ailments::Death>(neighbours[0])); // immune to fire
    ASSERT_TRUE(core->try_get<ailments::Death>(neighbours[1])); // resistant to fire
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
