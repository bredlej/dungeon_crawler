//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAIN_VIEW_HPP
#define DUNGEON_CRAWLER_MAIN_VIEW_HPP
extern "C" {
#include <raylib.h>
};
#include <core.hpp>
#include <map_view.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <memory>
#include <main_menu.hpp>

class MainView : public UIView {
public:
    explicit MainView(std::shared_ptr<Core> &core) : UIView(core) {};
    void render() override;
    void update() override;
    ~MainView() override = default;
private:
    Rectangle _dimension{0, 0, 1024, 768};
    MainMenu _main_menu;
};
#endif//DUNGEON_CRAWLER_MAIN_VIEW_HPP
