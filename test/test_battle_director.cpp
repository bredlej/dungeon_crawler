//
// Created by Patryk Szczypień on 15/08/2023.
//
#include "engine/battle_system/battle_director.hpp"
#include <gtest/gtest.h>
#include <memory>

class BattleDirectorTest : public testing::Test {
protected:
    BattleDirectorTest() = default;
};

static void process_turn(auto &battleDirector) {
    battleDirector.update(); // INACTIVE -> BATTLE_START
    battleDirector.update(); // BATTLE_START -> TURN_START
    battleDirector.update(); // TURN_START -> PLAYER_ACTIONS
    battleDirector.update(); // PLAYER_ACTIONS -> AI_ACTIONS
    battleDirector.update(); // AI_ACTIONS -> TURN_END
}

TEST_F(BattleDirectorTest, Test_Battle_Director_Initializes) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    BattleDirector battleDirector{core};
    battleDirector.update(); // INACTIVE -> BATTLE_START
    ASSERT_EQ(battleDirector.get_battle_phase(), BattlePhase::BATTLE_START);
}

TEST_F(BattleDirectorTest, Test_Battle_Goes_Through_All_Phases) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    BattleDirector battleDirector{core};

    process_turn(battleDirector);
    battleDirector.update(); // TURN_END -> BATTLE_END
    battleDirector.update(); // BATTLE_END -> FINISHED

    const auto battle_phase = battleDirector.get_battle_phase();
    ASSERT_EQ(battle_phase, BattlePhase::FINISHED);
}

TEST_F(BattleDirectorTest, Test_Battle_Ends_With_End_Condition) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    BattleDirector battle_director{core};

    struct TurnCounter {
        int32_t turn = 0;
    };
    core->registry.ctx().emplace<TurnCounter>();

    battle_director.pre_phase[BattlePhase::TURN_START] = [](const std::shared_ptr<Core> &core) {
        auto *turn_counter = core->registry.ctx().find<TurnCounter>();
        turn_counter->turn++;
    };
    battle_director.end_condition = [](const std::shared_ptr<Core> &core) {
        auto *turn_counter = core->registry.ctx().find<TurnCounter>();
        return turn_counter->turn == 2;
    };

    process_turn(battle_director); // Turn #1
    process_turn(battle_director); // Turn #2

    ASSERT_EQ(battle_director.get_battle_phase(), BattlePhase::BATTLE_END);
    ASSERT_EQ(core->registry.ctx().find<TurnCounter>()->turn, 2);
}

TEST_F(BattleDirectorTest, Test_Switches_To_Next_Phase_When_Guard_Is_True) {
    std::shared_ptr<Core> core = std::make_shared<Core>();
    BattleDirector battle_director{core};

    struct PlayerActionCounter {
        int32_t player_actions = 3;
    };
    core->registry.ctx().emplace<PlayerActionCounter>();

    battle_director.phase[BattlePhase::PLAYER_ACTIONS] = [](const std::shared_ptr<Core> &core) {
        auto *player_action_counter = core->registry.ctx().find<PlayerActionCounter>();
        player_action_counter->player_actions--;
        core->dispatcher.trigger(NextStateEvent{BattlePhase::PLAYER_ACTIONS});
    };

    battle_director.guard[BattlePhase::AI_ACTIONS] = [](const std::shared_ptr<Core> &core) {
        auto *player_action_counter = core->registry.ctx().find<PlayerActionCounter>();
        return player_action_counter->player_actions == 0;
    };

    battle_director.update(); // INACTIVE -> BATTLE_START
    battle_director.update(); // BATTLE_START -> TURN_START
    battle_director.update(); // TURN_START -> PLAYER_ACTIONS
    battle_director.update(); // PLAYER_ACTIONS -> PLAYER_ACTIONS - Action #1
    battle_director.update(); // PLAYER_ACTIONS -> PLAYER_ACTIONS - Action #2
    battle_director.update(); // PLAYER_ACTIONS -> PLAYER_ACTIONS - Action #3
    battle_director.update(); // PLAYER_ACTIONS -> AI_ACTIONS

    ASSERT_EQ(battle_director.get_battle_phase(), BattlePhase::AI_ACTIONS);
}

TEST_F(BattleDirectorTest, BattleDirector_Inside_Registry) {
    std::shared_ptr<Core> core = std::make_shared<Core>();

    core->registry.ctx().emplace<BattleDirector>(core);
    core->registry.ctx().find<BattleDirector>()->update();

    ASSERT_EQ(BattlePhase::BATTLE_START, core->registry.ctx().find<BattleDirector>()->get_battle_phase());
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}