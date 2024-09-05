//
// Created by Patryk Szczypień on 23/08/2024.
//

#ifndef DUNGEON_CRAWLER_BATTLE_CONFIGURATIONS_HPP
#define DUNGEON_CRAWLER_BATTLE_CONFIGURATIONS_HPP

#include <ecs/components.hpp>
#include <ecs/types.hpp>
#include <engine/events.hpp>

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

    template<int COLUMNS>
    static BattlePhaseConfiguration enemies_in_two_rows() {
        static_assert(COLUMNS > 0, "COLUMNS must be greater than 0");
        using namespace components::battle;
        return {
                .phase = {
                        {types::battle::BattlePhase::INACTIVE, [](const std::shared_ptr<Core> &core) {
                             core->registry.ctx().erase<config::BattleContext>();

                             auto spectre = core->registry.create();
                             core->registry.emplace<components::general::Name>(spectre, "Spectre");
                             core->registry.emplace<components::battle::enemies::EnemyType>(spectre, MonsterType::SPECTRE);

                             auto nomad = core->registry.create();
                                core->registry.emplace<components::general::Name>(nomad, "Nomad");
                                core->registry.emplace<components::battle::enemies::EnemyType>(nomad, MonsterType::NOMAD_THIEF);
                             auto ghoul = core->registry.create();
                                core->registry.emplace<components::general::Name>(ghoul, "Ghoul");
                                core->registry.emplace<components::battle::enemies::EnemyType>(ghoul, MonsterType::GHOUL);

                                auto two_rows = placement::TwoRows<COLUMNS>{};
                             two_rows.rows[placement::Row::BACK_ROW] = {nomad, ghoul, spectre};
                             two_rows.rows[placement::Row::FRONT_ROW] = {ghoul, spectre, nomad};

                             auto enemy_config = core->registry.create();
                             core->registry.emplace<placement::TwoRows<COLUMNS>>(enemy_config, two_rows);

                             auto battle_ctx = core->registry.create();
                             core->registry.ctx().emplace<config::BattleContext>(battle_ctx);
                             core->registry.emplace<config::EnemyConfig>(battle_ctx, enemy_config);
                             core->dispatcher.trigger(events::battle::NextStateEvent{types::battle::BattlePhase::INACTIVE});
                         }},
                        {types::battle::BattlePhase::BATTLE_START, [](const std::shared_ptr<Core> &core) {
                             if (auto *battle_ctx = core->registry.ctx().find<config::BattleContext>()) {
                                 auto enemy_config = core->registry.get<config::EnemyConfig>(battle_ctx->entity);
                                 auto two_rows = core->registry.get<placement::TwoRows<COLUMNS>>(enemy_config.entity);
                                 std::printf("Enemy placement:\n");
                                 std::printf("Back row:\n");
                                 for (const auto &entity: two_rows.rows[placement::Row::BACK_ROW]) {
                                     if (entity != entt::null) {
                                         auto &name = core->registry.get<components::general::Name>(entity);
                                         std::printf("%s\n", name.name.data());
                                     } else {
                                         std::printf("Empty\n");
                                     }
                                 }
                                 std::printf("Front row:\n");
                                 for (const auto &entity: two_rows.rows[placement::Row::FRONT_ROW]) {
                                     if (entity != entt::null) {
                                         auto &name = core->registry.get<components::general::Name>(entity);
                                         std::printf("%s\n", name.name.data());
                                     } else {
                                         std::printf("Empty\n");
                                     }
                                 }
                             }
                             core->dispatcher.trigger(events::battle::NextStateEvent{types::battle::BattlePhase::BATTLE_START});
                         }},
                }};
    }
    static BattlePhaseConfiguration animated_intro() {
        return {
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
}// namespace battle_configurations

#endif//DUNGEON_CRAWLER_BATTLE_CONFIGURATIONS_HPP
