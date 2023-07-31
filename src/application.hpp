//
// Created by geoco on 17.11.2022.
//
#ifndef DUNGEON_CRAWLER_APPLICATION_HPP
#define DUNGEON_CRAWLER_APPLICATION_HPP
#include <colors.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
extern "C" {
#include <raylib.h>
}
#include "views/dungeon_view.hpp"
#include "views/main_menu.hpp"
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <engine/assets.hpp>
#include <engine/core.hpp>
#include <string_view>

struct Config {
    static constexpr std::string_view title = "Dungeon Crawler";
    static constexpr struct {
        uint32_t width;
        uint32_t height;
    } window{1024, 768};
};

enum class ViewMode {
    MainMenu,
    Dungeon
};

class Application {
public:
    explicit Application() noexcept : _view_mode{ViewMode::Dungeon}, _core{std::make_shared<Core>()} {
        main_menu_view = std::make_unique<MainMenu>(_core);
    };
    Application(const Application &) noexcept = delete;
    Application(Application &) noexcept = delete;
    Application(Application &&) noexcept = delete;
    Application &operator=(const Application &) noexcept = delete;
    Application &operator=(const Application &&) noexcept = delete;

    void run() noexcept;

private:
    ViewMode _view_mode;
    std::shared_ptr<Core> _core;
    std::unique_ptr<DungeonView> dungeon_view;
    std::unique_ptr<MainMenu> main_menu_view;
    static void _toggle_fullscreen() noexcept;
};
#endif//DUNGEON_CRAWLER_APPLICATION_HPP
