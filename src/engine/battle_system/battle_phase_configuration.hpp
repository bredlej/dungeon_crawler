//
// Created by Patryk Szczypień on 23/08/2024.
//

#ifndef DUNGEON_CRAWLER_BATTLE_CONFIGURATIONS_HPP
#define DUNGEON_CRAWLER_BATTLE_CONFIGURATIONS_HPP

#include <engine/events.hpp>
#include <ecs/types.hpp>
#include <ecs/components.hpp>

using namespace battle;
using namespace events::battle;

using void_func = std::function<void(std::shared_ptr<Core>)>;
using bool_func = std::function<bool(std::shared_ptr<Core>)>;
using void_map = std::unordered_map<BattlePhase, void_func>;
using bool_map = std::unordered_map<BattlePhase, bool_func>;

class BattlePhaseConfiguration {
public:

    void_map pre_phase;
    void_map phase;
    void_map post_phase;
    bool_map guard;
    bool_func end_condition;
};
namespace battle_configurations {
    static BattlePhaseConfiguration default_config() {
        return {
                .pre_phase = {
                        {BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::BATTLE_START, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::TURN_START, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::PLAYER_ACTIONS, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::AI_ACTIONS, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::TURN_END, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::BATTLE_END, [](const std::shared_ptr<Core> &core) {}},
                },
                .phase = {
                        {BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::INACTIVE}); }},
                        {BattlePhase::BATTLE_START, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::BATTLE_START}); }},
                        {BattlePhase::TURN_START, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::TURN_START}); }},
                        {BattlePhase::PLAYER_ACTIONS, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::PLAYER_ACTIONS}); }},
                        {BattlePhase::AI_ACTIONS, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::AI_ACTIONS}); }},
                        {BattlePhase::TURN_END, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::TURN_END}); }},
                        {BattlePhase::BATTLE_END, [](const std::shared_ptr<Core> &core) { core->dispatcher.trigger(NextStateEvent{BattlePhase::BATTLE_END}); }},
                        {BattlePhase::FINISHED, [](const std::shared_ptr<Core> &core) {}},
                },
                .post_phase = {
                        {BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::BATTLE_START, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::TURN_START, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::PLAYER_ACTIONS, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::AI_ACTIONS, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::TURN_END, [](const std::shared_ptr<Core> &core) {}},
                        {BattlePhase::BATTLE_END, [](const std::shared_ptr<Core> &core) {}},
                },
                .guard = {
                        {BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) { return true; }},
                        {BattlePhase::BATTLE_START, [](const std::shared_ptr<Core> &core) { return true; }},
                        {BattlePhase::TURN_START, [](const std::shared_ptr<Core> &core) { return true; }},
                        {BattlePhase::PLAYER_ACTIONS, [](const std::shared_ptr<Core> &core) { return true; }},
                        {BattlePhase::AI_ACTIONS, [](const std::shared_ptr<Core> &core) { return true; }},
                        {BattlePhase::TURN_END, [](const std::shared_ptr<Core> &core) { return true; }},
                        {BattlePhase::BATTLE_END, [](const std::shared_ptr<Core> &core) { return true; }},
                },
                .end_condition = [](const std::shared_ptr<Core> &core) { return true; }};
    }
    static BattlePhaseConfiguration animated_intro() {
        return {
                .phase = {
                        {types::battle::BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) {
                             if (auto *animation = core->registry.ctx().find<components::values::AnimationTimer>()) {
                                 if (animation->counter <= 0) {
                                     core->registry.ctx().erase<components::values::AnimationTimer>();
                                     core->dispatcher.trigger(NextStateEvent{BattlePhase::INACTIVE});
                                 }
                             }
                         }}},
                .post_phase = {{types::battle::BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) {
                                    std::printf("Starting battle...");
                                }},
                               {types::battle::BattlePhase::BATTLE_END, [](const std::shared_ptr<Core> &core) {
                                    if (core->registry.ctx().contains<components::values::Encounter>()) {
                                        core->registry.ctx().erase<components::values::Encounter>();
                                    }
                                    std::printf("Ending battle...");
                                }}},
                .guard = {
                        {types::battle::BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) {
                             return !core->registry.ctx().contains<components::values::AnimationTimer>();
                         }},
                },
                .end_condition = [](const std::shared_ptr<Core> &core) {
                    return true;
                }};
               }
}

#endif//DUNGEON_CRAWLER_BATTLE_CONFIGURATIONS_HPP
