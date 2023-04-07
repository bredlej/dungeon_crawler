//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAIN_MENU_HPP
#define DUNGEON_CRAWLER_MAIN_MENU_HPP
extern "C" {
#include <raylib.h>
};
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>

class MainMenu {
public:
    explicit MainMenu() = default;
    ~MainMenu() = default;
    static void render();
    void update();
};
#endif//DUNGEON_CRAWLER_MAIN_MENU_HPP
