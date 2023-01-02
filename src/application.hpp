//
// Created by geoco on 17.11.2022.
//
#ifndef DUNGEON_CRAWLER_APPLICATION_HPP
#define DUNGEON_CRAWLER_APPLICATION_HPP
extern "C" {
#include <raylib.h>
}
#include <engine/assets.hpp>
#include <engine/core.hpp>
#include "views/dungeon_view.hpp"
#include "views/main_menu.hpp"
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <unordered_map>

struct Config {
    static constexpr std::string_view title = "Dungeon Crawler";
    static constexpr struct {
        uint32_t width;
        uint32_t height;
    } window{1024, 800};
};

enum class ViewMode {
    MainMenu, Dungeon
};

using ViewMap = std::unordered_map<ViewMode, std::unique_ptr<UIView>>;

class Application {
public:
    explicit Application() noexcept : _view_mode{ViewMode::Dungeon}, _core{std::make_shared<Core>()} {};
    Application(const Application &) noexcept = delete;
    Application(Application &) noexcept = delete;
    Application(Application &&) noexcept = delete;
    Application &operator=(const Application &) noexcept = delete;
    Application &operator=(const Application &&) noexcept = delete;

    void run();
private:
    ViewMode _view_mode;
    ViewMap _views;
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_APPLICATION_HPP
