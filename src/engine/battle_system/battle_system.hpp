//
// Created by Patryk Szczypień on 14/10/2023.
//

#ifndef DUNGEON_CRAWLER_BATTLE_SYSTEM_HPP
#define DUNGEON_CRAWLER_BATTLE_SYSTEM_HPP
#include <battle_system/combat.hpp>
#include <battle_system/battle_director.hpp>
#include <memory>
#include <core.hpp>
#include <vector>

struct Battle {
    std::unique_ptr<Combat> combat;
    std::unique_ptr<BattleDirector> battle_director;
    entt::entity battle_entity;
};

class BattleSystem {
public:
    explicit BattleSystem(const std::shared_ptr<Core>& core) : _core{core} {};
    BattleSystem(const BattleSystem &) = delete;
    BattleSystem(BattleSystem &&) = default;
    BattleSystem &operator=(const BattleSystem &) = delete;
    BattleSystem &operator=(BattleSystem &&) = delete;
    ~BattleSystem() = default;

    Battle start_battle();
private:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_BATTLE_SYSTEM_HPP
