//
// Created by Patryk Szczypień on 14/10/2023.
//
#include <engine/battle_system/battle_system.hpp>
#include <engine/parsers/monster_parser.hpp>
#include <engine/parsers/skill_parser.hpp>
#include <engine/skills.hpp>
#include <engine/monsters.hpp>
#include <gtest/gtest.h>

class BattleSystemTest : public ::testing::Test {
protected:
    BattleSystemTest() {}
    std::shared_ptr<Core> core = std::make_shared<Core>();
    BattleSystem battle_system = BattleSystem(core);
};

TEST_F(BattleSystemTest, Test_Battle_System_Starts_Battle) {
    auto battle = battle_system.start_battle();
    ASSERT_TRUE(battle.combat != nullptr);
    ASSERT_TRUE(battle.battle_director != nullptr);
    ASSERT_TRUE(battle.battle_entity != entt::null);
}

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
    std::printf("Created character %s with %.0f hit points (id=%u)\n", name.data(), hit_points, character);
    return character;
}

TEST_F(BattleSystemTest, Test_Battle_Finishes_When_Enemies_Are_Killed) {
    auto &registry = core->registry;

    auto attacker = create_character(core, "Attacker", 10);

    auto enemy1 = create_character(core, "Enemy 1", 10);
    registry.emplace<components::general::EnemyMarker>(enemy1);

    auto enemy2 = create_character(core, "Enemy 2", 15);
    registry.emplace<components::general::EnemyMarker>(enemy2);

    auto battle = battle_system.start_battle();
    auto skill_json = SkillParser::parse("resources/test_skills.json");
    core->registry.ctx().emplace<skills::SkillsMap>(skills::SkillsMap::from_json(skill_json));


    auto &combat = battle.combat;
    const auto &enemies = std::vector{{enemy1, enemy2}};

    battle.battle_director->end_condition = [&enemies](const std::shared_ptr<Core> &core) {
        auto &registry = core->registry;
        for (const auto entity: enemies) {
            auto &attributes = registry.get<Attributes>(entity);
            if (attributes.attributes[character::Attribute::HIT_POINTS] > 0) {
                return false;
            }
        }
        return true;
    };

    battle.battle_director->pre_phase[BattlePhase::TURN_START] = [](const std::shared_ptr<Core> &core) {
        std::printf("\n New turn\n");
        auto &registry = core->registry;
        auto view = registry.view<components::general::Name, Attributes>();
        for (auto const entity: view) {
            auto attributes = view.get<Attributes>(entity);
            auto name = view.get<components::general::Name>(entity);
            std::printf("%s has %.0f hit points\n", name.name.data(), attributes.attributes[character::Attribute::HIT_POINTS]);
        }
    };

    battle.battle_director->phase[BattlePhase::PLAYER_ACTIONS] = [&attacker, &combat, &enemies](const std::shared_ptr<Core> &core) {
        auto &registry = core->registry;

        for (auto const entity: enemies) {
            if (auto &[attributes] = registry.get<Attributes>(entity); attributes[character::Attribute::HIT_POINTS] > 0) {
                auto fireball_func = skills::SkillsMap::instance_offensive_skill(core, *core->registry.ctx().find<skills::SkillsMap>(), "FIREBALL");
                const auto fireball = fireball_func(core);
                core->registry.emplace_or_replace<targets::TargetSingle>(fireball, entity, 1.0f);
                combat->_combat_dispatcher.trigger(events::battle::AttackEvent{attacker, fireball});
            }
        }

        core->dispatcher.trigger(NextStateEvent{BattlePhase::PLAYER_ACTIONS});
    };

    ASSERT_EQ(BattlePhase::INACTIVE, battle.battle_director.get()->get_battle_phase());
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();
    battle.battle_director->update();

    ASSERT_EQ(BattlePhase::FINISHED, battle.battle_director.get()->get_battle_phase());
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}