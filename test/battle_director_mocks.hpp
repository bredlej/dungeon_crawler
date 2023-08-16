//
// Created by Patryk Szczypień on 16/08/2023.
//

#ifndef DUNGEON_CRAWLER_BATTLE_DIRECTOR_MOCKS_HPP
#define DUNGEON_CRAWLER_BATTLE_DIRECTOR_MOCKS_HPP

static std::string battle_phase_to_string(BattlePhase phase) {
    switch (phase) {
        case BattlePhase::INACTIVE:
            return "INACTIVE";
        case BattlePhase::BATTLE_START:
            return "BATTLE_START";
        case BattlePhase::TURN_START:
            return "TURN_START";
        case BattlePhase::PLAYER_ACTIONS:
            return "PLAYER_ACTIONS";
        case BattlePhase::AI_ACTIONS:
            return "AI_ACTIONS";
        case BattlePhase::TURN_END:
            return "TURN_END";
        case BattlePhase::BATTLE_END:
            return "BATTLE_END";
        case BattlePhase::FINISHED:
            return "FINISHED";
    }
}
class TestBattleDirector : public BattleDirector<TestBattleDirector> {
    friend class BattleDirector<TestBattleDirector>;

public:
    explicit TestBattleDirector(std::shared_ptr<Core> &core) : BattleDirector<TestBattleDirector>(core) {
        _core->dispatcher.sink<NextStateEvent>().connect<&TestBattleDirector::next_state>(this);
    };

    constexpr void update() noexcept override {
        switch (_battle_phase) {
            case BattlePhase::BATTLE_START:
                battle_start();
                break;
            case BattlePhase::TURN_START:
                turn_start();
                break;
            case BattlePhase::PLAYER_ACTIONS:
                player_actions();
                break;
            case BattlePhase::AI_ACTIONS:
                ai_actions();
                break;
            case BattlePhase::TURN_END:
                turn_end();
                break;
            case BattlePhase::BATTLE_END:
                battle_end();
                break;
            case BattlePhase::INACTIVE:
                initialize();
                break;
            case BattlePhase::FINISHED:
                finish();
                break;
        }
        _core->dispatcher.update();
    }

protected:
    constexpr void next_state(const NextStateEvent &next_state_event) noexcept override {
        switch (next_state_event.from_phase) {
            case BattlePhase::BATTLE_START:
                if (turn_start_guard()) {
                    post_battle_start();
                    pre_turn_start();
                    _battle_phase = BattlePhase::TURN_START;
                }
                break;
            case BattlePhase::TURN_START:
                if (player_actions_guard()) {
                    post_turn_start();
                    pre_player_actions();
                    _battle_phase = BattlePhase::PLAYER_ACTIONS;
                }
                break;
            case BattlePhase::PLAYER_ACTIONS:
                if (ai_actions_guard()) {
                    post_player_actions();
                    pre_ai_actions();
                    _battle_phase = BattlePhase::AI_ACTIONS;
                }
                break;
            case BattlePhase::AI_ACTIONS:
                if (turn_end_guard()) {
                    post_ai_actions();
                    pre_turn_end();
                    _battle_phase = BattlePhase::TURN_END;
                }
                break;
            case BattlePhase::TURN_END:
                if (battle_end_guard()) {
                    post_turn_end();
                    if (end_condition()) {
                        pre_battle_end();
                        _battle_phase = BattlePhase::BATTLE_END;
                    } else {
                        pre_turn_start();
                        _battle_phase = BattlePhase::TURN_START;
                    }
                }
                break;
            case BattlePhase::BATTLE_END:
                post_battle_end();
                _battle_phase = BattlePhase::FINISHED;
                break;
            case BattlePhase::INACTIVE:
                if (battle_start_guard()) {
                    pre_battle_start();
                    _battle_phase = BattlePhase::BATTLE_START;
                }
                break;
            case BattlePhase::FINISHED:
                break;
        }
    }

    constexpr void initialize() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::INACTIVE});
    };
    constexpr void pre_battle_start() noexcept override {};
    constexpr void battle_start() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::BATTLE_START});
    };
    constexpr void post_battle_start() noexcept override{};
    constexpr bool battle_start_guard() noexcept override { return true; };

    constexpr void pre_turn_start() noexcept override{};
    constexpr void turn_start() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::TURN_START});
    };
    constexpr void post_turn_start() noexcept override{};
    constexpr bool turn_start_guard() noexcept override { return true; };

    constexpr void pre_player_actions() noexcept override{};
    constexpr void player_actions() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::PLAYER_ACTIONS});
    };
    constexpr void post_player_actions() noexcept override{};
    constexpr bool player_actions_guard() noexcept override { return true; };

    constexpr void pre_ai_actions() noexcept override{};
    constexpr void ai_actions() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::AI_ACTIONS});
    };
    constexpr void post_ai_actions() noexcept override{};
    constexpr bool ai_actions_guard() noexcept override { return true; };

    constexpr void pre_turn_end() noexcept override{};
    constexpr void turn_end() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::TURN_END});
    };
    constexpr void post_turn_end() noexcept override{};
    constexpr bool turn_end_guard() noexcept override { return true; };

    constexpr void pre_battle_end() noexcept override{};
    constexpr void battle_end() noexcept override {
        _core->dispatcher.trigger(NextStateEvent{BattlePhase::BATTLE_END});
    };
    constexpr void post_battle_end() noexcept override{};
    constexpr bool battle_end_guard() noexcept override { return true; };

    constexpr bool end_condition() noexcept override { return true; };

    constexpr void finish() noexcept override{};
};

class TestBattleDirector_TurnStartGuard : public TestBattleDirector {
    friend class BattleDirector<TestBattleDirector_TurnStartGuard>;

public:
    explicit TestBattleDirector_TurnStartGuard(std::shared_ptr<Core> &core) : TestBattleDirector(core) {};
    using TestBattleDirector::TestBattleDirector;

protected:
    constexpr bool turn_start_guard() noexcept override { return false; };
};

struct TurnCounter {
    int turn{0};
};

class TestBattleDirector_Finishes_After_Turn_2 : public TestBattleDirector {
    friend class BattleDirector<TestBattleDirector_Finishes_After_Turn_2>;
public:
    explicit TestBattleDirector_Finishes_After_Turn_2(std::shared_ptr<Core> &core) : TestBattleDirector(core) {};
    using TestBattleDirector::TestBattleDirector;

    int get_turn() noexcept {
        return _core->registry.ctx().find<TurnCounter>()->turn;
    };
protected:
    void initialize() noexcept override {
        _core->registry.ctx().emplace<TurnCounter>();
        _core->dispatcher.enqueue(NextStateEvent{BattlePhase::INACTIVE});
    };
    void pre_turn_start() noexcept override {
        _core->registry.ctx().find<TurnCounter>()->turn++;
    };
    bool end_condition() noexcept override {
        return _core->registry.ctx().find<TurnCounter>()->turn == 2;
    };
};
#endif//DUNGEON_CRAWLER_BATTLE_DIRECTOR_MOCKS_HPP
