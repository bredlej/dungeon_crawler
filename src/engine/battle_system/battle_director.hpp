//
// Created by Patryk Szczypień on 15/08/2023.
//

#ifndef DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
#define DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
#include "ecs/types.hpp"
#include "engine/events.hpp"
#include "engine/core.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <battle_system/battle_phase_configuration.hpp>

using namespace battle;
using void_func = std::function<void(std::shared_ptr<Core>)>;
using bool_func = std::function<bool(std::shared_ptr<Core>)>;
using void_map = std::unordered_map<BattlePhase, void_func>;
using bool_map = std::unordered_map<BattlePhase, bool_func>;


inline std::string to_string(BattlePhase battle_phase) {
    switch (battle_phase) {
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


/**
 * @class BattleDirector
 * @brief Coordinates the flow of a battle by managing different phases and their actions
 *
 * The BattleDirector class is responsible for managing the flow of a battle by coordinating
 * different phases and their actions. It keeps track of the current battle phase, allows for
 * updating the battle state, and handles transitions between phases based on certain conditions.
 */
class BattleDirector {
public:
    void_map pre_phase;
    void_map post_phase;
    void_map phase;
    bool_map guard;
    bool_func end_condition;

    explicit BattleDirector(const std::shared_ptr<Core> &core) : _core{core} {
        _core->dispatcher.sink<NextStateEvent>().connect<&BattleDirector::next_state>(*this);
        reset_to_default();
    };

    BattleDirector(const BattleDirector &) = default;
    BattleDirector(BattleDirector &&) = default;
    BattleDirector &operator=(const BattleDirector &) = default;
    BattleDirector &operator=(BattleDirector &&) = delete;

    ~BattleDirector() = default;

    void apply_configuration(const BattlePhaseConfiguration& config) {
        _battle_phase = types::battle::BattlePhase::INACTIVE;
        for (auto &[key, value]: config.pre_phase) {
            pre_phase[key] = value;
        }
        for (auto &[key, value]: config.post_phase) {
            post_phase[key] = value;
        }
        for (auto &[key, value]: config.phase) {
            phase[key] = value;
        }
        for (auto &[key, value]: config.guard) {
            guard[key] = value;
        }
        if (config.end_condition) {
            end_condition = config.end_condition;
        }
        else {
            end_condition = [](const std::shared_ptr<Core> &core) { return true; };
        }
    }

    [[nodiscard]] BattlePhase get_battle_phase() const noexcept {
        return _battle_phase;
    }

    constexpr void update() noexcept {
        phase[_battle_phase](_core);
    }

    constexpr void reset_to_default() noexcept {
        apply_configuration(battle_configurations::default_config());
    }

    static constexpr void guard_and_process(BattleDirector &battleDirector, std::shared_ptr<Core> &core, BattlePhase from_phase, BattlePhase to_phase) {
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
                    post_phase[BattlePhase::INACTIVE](_core);
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
