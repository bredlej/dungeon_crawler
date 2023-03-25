//
// Created by Patryk Szczypień on 20/03/2023.
//

#ifndef DUNGEON_CRAWLER_DUNGEON_UI_H
#define DUNGEON_CRAWLER_DUNGEON_UI_H
#include <memory>
#include <engine/core.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <events.hpp>
#include <components.hpp>
#include <colors.hpp>
class DungeonUI {
public:
    explicit DungeonUI(const std::shared_ptr<Core>& core) : _core{core} {_initialize();};
    DungeonUI(const DungeonUI &) noexcept = delete;
    DungeonUI(DungeonUI &) noexcept = delete;
    DungeonUI(DungeonUI &&other) noexcept = delete;
    DungeonUI &operator=(const DungeonUI &) noexcept = delete;
    DungeonUI &operator=(const DungeonUI &&) noexcept = delete;
    void render();
private:
    std::shared_ptr<Core> _core;
    void _initialize();
    bool _show_demo{false};
    bool _bottom_window_visible{false};
    bool _encounter_chance_window_visible{false};
    void _toggle_demo();
    void _render_movement_buttons();
    void _render_encounter_chance();
};
#endif//DUNGEON_CRAWLER_DUNGEON_UI_H
