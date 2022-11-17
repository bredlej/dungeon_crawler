//
// Created by geoco on 17.11.2022.
//
#ifndef DUNGEON_CRAWLER_APPLICATION_HPP
#define DUNGEON_CRAWLER_APPLICATION_HPP
extern "C" {
#include <raylib.h>
}
#include <Core.hpp>
#include <MainMenu.hpp>
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <unordered_map>

struct Config {
    static constexpr std::string_view title = "Dungeon Crawler";
    static constexpr struct {
        uint32_t width;
        uint32_t height;
    } window{640, 480};
};

enum class ViewMode {
    MainMenu
};

using ViewMap = std::unordered_map<ViewMode, std::unique_ptr<UIView>>;

class Application {
public:
    explicit Application() noexcept : _view_mode{ViewMode::MainMenu} {};
    Application(const Application &) noexcept = delete;
    Application(Application &) noexcept = delete;
    Application(Application &&) noexcept = delete;
    Application &operator=(const Application &) noexcept = delete;
    Application &operator=(const Application &&) noexcept = delete;

    void run();

private:
    ViewMode _view_mode;
    ViewMap _views;
};
#endif//DUNGEON_CRAWLER_APPLICATION_HPP
