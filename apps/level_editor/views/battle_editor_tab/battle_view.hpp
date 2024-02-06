//
// Created by Patryk Szczypień on 11/08/2023.
//

#ifndef DUNGEON_CRAWLER_BATTLE_VIEW_HPP
#define DUNGEON_CRAWLER_BATTLE_VIEW_HPP
#include <battle_system/battle_system.hpp>
extern "C" {
#include "raylib-4.0.0/src/raylib.h"
};
#include "engine/battle_system/battle_director.hpp"
#include "engine/core.hpp"
#include "editor_events.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include <editor_assets.hpp>
#include <memory>

class BattleView : public UIView<BattleView> {
public:
    explicit BattleView(std::shared_ptr<Core> core) : UIView{core} ,_battle_system{core} {};
    BattleView(const BattleView &) = delete;
    BattleView(BattleView &&) = delete;
    BattleView &operator=(const BattleView &) = delete;
    BattleView &operator=(BattleView &&) = delete;
    ~BattleView() override = default;
    void render() noexcept override;
    void update() noexcept override;

private:
    BattleSystem _battle_system;
};
#endif//DUNGEON_CRAWLER_BATTLE_VIEW_HPP
