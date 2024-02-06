//
// Created by Patryk Szczypień on 14/10/2023.
//
#include <battle_system/battle_system.hpp>

Battle BattleSystem::start_battle() {
    auto battle = Battle();
    battle.combat = std::make_unique<Combat>(_core);
    battle.battle_director = std::make_unique<BattleDirector>(_core);
    battle.battle_entity = _core->registry.create();
    return battle;
}