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
#include "encounter_view.hpp"
#include "party_view.hpp"
#include "views/dungeon_view.hpp"
#include "views/main_menu.hpp"
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <engine/assets.hpp>
#include <engine/battle_system/battle_director.hpp>
#include <engine/core.hpp>
#include <scheduler.hpp>
#include <string_view>

struct Config {
    static constexpr std::string_view title = "Dungeon Crawler Engine";
    static constexpr struct {
        uint32_t width;
        uint32_t height;
    } window{1024, 768};
};

enum class ViewMode {
    MainMenu,
    Dungeon,
    Encounter,
    Party
};

class Application {
public:
    explicit Application() noexcept : _view_mode{ViewMode::Dungeon}, _core{std::make_shared<Core>()}, _battle_director(_core) {
        party_view = std::make_unique<PartyView>(_core);
        encounter_view = std::make_unique<EncounterView>(_core, &_battle_director);
    };
    Application(const Application &) noexcept = delete;
    Application(Application &) noexcept = delete;
    Application(Application &&) noexcept = delete;
    Application &operator=(const Application &) noexcept = delete;
    Application &operator=(const Application &&) noexcept = delete;

    void run() noexcept;
    void common_update() noexcept;
    void initialize() noexcept;
    void start_encounter(events::dungeon::StartEncounter &event) noexcept;
    void end_encounter(const events::dungeon::EndEncounter &event) noexcept;
    void show_party_view() noexcept;
    void show_dungeon_view() noexcept;
private:
    ViewMode _view_mode;
    std::shared_ptr<Core> _core;
    BattleDirector _battle_director;
    std::unique_ptr<DungeonView> dungeon_view;
    std::unique_ptr<MainMenu> main_menu_view;
    std::unique_ptr<EncounterView> encounter_view;
    std::unique_ptr<PartyView> party_view;
    static void _toggle_fullscreen() noexcept;
};
#endif//DUNGEON_CRAWLER_APPLICATION_HPP
