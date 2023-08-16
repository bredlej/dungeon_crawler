//
// Created by Patryk Szczypień on 15/08/2023.
//

#ifndef DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
#define DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
#include <engine/core.hpp>
#include <memory>
#include <functional>
#include <unordered_map>

enum class BattlePhase {
    INACTIVE,
    BATTLE_START,
    TURN_START,
    PLAYER_ACTIONS,
    AI_ACTIONS,
    TURN_END,
    BATTLE_END,
    FINISHED
};

struct NextStateEvent {
    BattlePhase from_phase;
};

class CustomBattleDirector {
    using void_func = std::function<void(std::shared_ptr<Core>)>;
    using bool_func = std::function<bool(std::shared_ptr<Core>)>;
    using void_map = std::unordered_map<BattlePhase, void_func>;
    using bool_map = std::unordered_map<BattlePhase, bool_func>;

public:
    void_map pre_phase;
    void_map post_phase;
    void_map phase;
    bool_map guard;
    bool_func end_condition;

    explicit CustomBattleDirector(std::shared_ptr<Core> &core) : _core{core} {
        _core->dispatcher.sink<NextStateEvent>().connect<&CustomBattleDirector::next_state>(this);

        pre_phase[BattlePhase::INACTIVE] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::INACTIVE] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::INACTIVE] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::INACTIVE}); };
        guard[BattlePhase::INACTIVE] = [](const std::shared_ptr<Core> &core) { return true; };

        pre_phase[BattlePhase::BATTLE_START] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::BATTLE_START] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::BATTLE_START] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::BATTLE_START}); };
        guard[BattlePhase::BATTLE_START] = [](const std::shared_ptr<Core> &core) { return true; };

        pre_phase[BattlePhase::TURN_START] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::TURN_START] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::TURN_START] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::TURN_START}); };
        guard[BattlePhase::TURN_START] = [](const std::shared_ptr<Core> &core) { return true; };

        pre_phase[BattlePhase::PLAYER_ACTIONS] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::PLAYER_ACTIONS] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::PLAYER_ACTIONS] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::PLAYER_ACTIONS}); };
        guard[BattlePhase::PLAYER_ACTIONS] = [](const std::shared_ptr<Core> &core) { return true; };

        pre_phase[BattlePhase::AI_ACTIONS] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::AI_ACTIONS] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::AI_ACTIONS] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::AI_ACTIONS}); };
        guard[BattlePhase::AI_ACTIONS] = [](const std::shared_ptr<Core> &core) { return true; };

        pre_phase[BattlePhase::TURN_END] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::TURN_END] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::TURN_END] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::TURN_END}); };
        guard[BattlePhase::TURN_END] = [](const std::shared_ptr<Core> &core) { return true; };

        pre_phase[BattlePhase::BATTLE_END] = [](const std::shared_ptr<Core> &core) {};
        post_phase[BattlePhase::BATTLE_END] = [](const std::shared_ptr<Core> &core) {};
        phase[BattlePhase::BATTLE_END] = [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::BATTLE_END}); };
        guard[BattlePhase::BATTLE_END] = [](const std::shared_ptr<Core> &core) { return true; };

        phase[BattlePhase::FINISHED] = [](const std::shared_ptr<Core> &core) {};

        end_condition = [](const std::shared_ptr<Core> &core) { return true; };
    };
    explicit CustomBattleDirector(std::shared_ptr<Core> &core, void_map &&pre_phase, void_map &&post_phase, void_map &&phase, bool_map &&guard, bool_func &&end_condition)
        : _core{core}, pre_phase{std::move(pre_phase)}, post_phase{std::move(post_phase)}, phase{std::move(phase)}, guard{std::move(guard)}, end_condition{std::move(end_condition)} {
        _core->dispatcher.sink<NextStateEvent>().connect<&CustomBattleDirector::next_state>(this);
    };

    ~CustomBattleDirector() = default;

    [[nodiscard]] BattlePhase get_battle_phase() const noexcept {
        return _battle_phase;
    }

    constexpr void update() noexcept {
        phase[_battle_phase](_core);
        _core->dispatcher.update();
    }

    static constexpr void guard_and_process(CustomBattleDirector &battleDirector, std::shared_ptr<Core> &core, BattlePhase from_phase, BattlePhase to_phase) {
        if (battleDirector.guard[from_phase](core)) {
            battleDirector.post_phase[from_phase](core);
            battleDirector.pre_phase[to_phase](core);
            battleDirector._battle_phase = to_phase;
        }
    }

    constexpr void next_state(const NextStateEvent &next_state_event) noexcept {
        switch (next_state_event.from_phase) {
            case BattlePhase::INACTIVE:
                if (guard[BattlePhase::INACTIVE](_core)) {
                    pre_phase[BattlePhase::BATTLE_START](_core);
                    _battle_phase = BattlePhase::BATTLE_START;
                }
                break;
            case BattlePhase::BATTLE_START:
                guard_and_process(*this, _core, BattlePhase::BATTLE_START, BattlePhase::TURN_START);
                break;
            case BattlePhase::TURN_START:
                guard_and_process(*this, _core, BattlePhase::TURN_START, BattlePhase::PLAYER_ACTIONS);
                break;
            case BattlePhase::PLAYER_ACTIONS:
                guard_and_process(*this, _core, BattlePhase::PLAYER_ACTIONS, BattlePhase::AI_ACTIONS);
                break;
            case BattlePhase::AI_ACTIONS:
                guard_and_process(*this, _core, BattlePhase::AI_ACTIONS, BattlePhase::TURN_END);
                break;
            case BattlePhase::TURN_END:
                if (guard[BattlePhase::TURN_END](_core)) {
                    post_phase[BattlePhase::TURN_END](_core);
                    if (end_condition(_core)) {
                        pre_phase[BattlePhase::BATTLE_END](_core);
                        _battle_phase = BattlePhase::BATTLE_END;
                    } else {
                        pre_phase[BattlePhase::TURN_START](_core);
                        _battle_phase = BattlePhase::TURN_START;
                    }
                }
                break;
            case BattlePhase::BATTLE_END:
                post_phase[BattlePhase::BATTLE_END](_core);
                _battle_phase = BattlePhase::FINISHED;
                break;
            case BattlePhase::FINISHED:
                break;
        }
    }

private:
    std::shared_ptr<Core> _core;
    BattlePhase _battle_phase{BattlePhase::INACTIVE};
};

#endif//DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP