//
// Created by Patryk Szczypień on 05/09/2024.
//

#ifndef DUNGEON_CRAWLER_ENCOUNTER_UI_HPP
#define DUNGEON_CRAWLER_ENCOUNTER_UI_HPP
#include <colors.hpp>
#include <components.hpp>
#include <engine/core.hpp>
#include <events.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <memory>

class EncounterUI {
public:
    explicit EncounterUI(const std::shared_ptr<Core> core) : _core{core} {
        _initialize();
    };
    EncounterUI(const EncounterUI &) noexcept = delete;
    EncounterUI(EncounterUI &) noexcept = delete;
    EncounterUI(EncounterUI &&other) noexcept = delete;
    EncounterUI &operator=(const EncounterUI &) noexcept = delete;
    EncounterUI &operator=(const EncounterUI &&) noexcept = delete;
    void render();

    [[nodiscard]] float back_row_x() const noexcept { return _back_row_x; }
    [[nodiscard]] float back_row_y() const noexcept { return _back_row_y; }
    [[nodiscard]] float front_row_x() const noexcept { return _front_row_x; }
    [[nodiscard]] float front_row_y() const noexcept { return _front_row_y; }
    [[nodiscard]] float offset() const noexcept { return _offset; }
    [[nodiscard]] float speed() const noexcept { return _speed; }
    [[nodiscard]] float edge_from() const noexcept { return _edge_from; }
    [[nodiscard]] float edge_to() const noexcept { return _edge_to; }
    void set_color(Color color) { _color = color; }
    [[nodiscard]] Color color() const noexcept { return _color; }

private:
    std::shared_ptr<Core> _core;
    void _initialize();
    bool _bottom_window_visible{false};
    float _back_row_x{72.0f};
    float _back_row_y{156.0f};
    float _front_row_x{60.0f};
    float _front_row_y{210.0f};
    float _offset{96.0f};
    float _speed = 0.002f;
    float _edge_from{1.0f};
    float _edge_to{0.9f};
    Color _color;
};
#endif//DUNGEON_CRAWLER_ENCOUNTER_UI_HPP
