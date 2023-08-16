//
// Created by Patryk Szczypień on 15/08/2023.
//
#include <core.hpp>
#include <engine/battle_director.hpp>
#include <gtest/gtest.h>
#include <memory>
#include "battle_director_mocks.hpp"

class BattleDirectorTest : public testing::Test {
protected:
    BattleDirectorTest() = default;
};

TEST_F(BattleDirectorTest, Test_Init) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    TestBattleDirector battleDirector{core};

    ASSERT_EQ(battleDirector.get_battle_phase(), BattlePhase::INACTIVE);
}

TEST_F(BattleDirectorTest, Test_Update) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    TestBattleDirector battleDirector{core};

    battleDirector.update();
    const auto battle_phase = battleDirector.get_battle_phase();
    ASSERT_EQ(battle_phase, BattlePhase::BATTLE_START);
}

TEST_F(BattleDirectorTest, Test_Guard_Prevents_Phase_Change) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    TestBattleDirector_TurnStartGuard battleDirector{core};

    battleDirector.update();
    battleDirector.update();
    const auto battle_phase = battleDirector.get_battle_phase();
    ASSERT_EQ(battle_phase, BattlePhase::BATTLE_START);
}

static void process_turn(auto &battleDirector) {
    battleDirector.update(); // INACTIVE -> BATTLE_START
    battleDirector.update(); // BATTLE_START -> TURN_START
    battleDirector.update(); // TURN_START -> PLAYER_ACTIONS
    battleDirector.update(); // PLAYER_ACTIONS -> AI_ACTIONS
    battleDirector.update(); // AI_ACTIONS -> TURN_END
}

TEST_F(BattleDirectorTest, Test_Battle_Reaches_Finished_Phase) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    TestBattleDirector battleDirector{core};

    process_turn(battleDirector);
    battleDirector.update(); // TURN_END -> BATTLE_END
    battleDirector.update(); // BATTLE_END -> FINISHED

    const auto battle_phase = battleDirector.get_battle_phase();
    ASSERT_EQ(battle_phase, BattlePhase::FINISHED);
}

TEST_F(BattleDirectorTest, Test_Battle_Ends_In_Turn_2) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    TestBattleDirector_Finishes_After_Turn_2 battleDirector{core};

    process_turn(battleDirector);
    process_turn(battleDirector);

    const auto battle_phase = battleDirector.get_battle_phase();
    const auto turn = battleDirector.get_turn();
    ASSERT_EQ(battle_phase, BattlePhase::BATTLE_END);
    ASSERT_EQ(turn, 2);
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}