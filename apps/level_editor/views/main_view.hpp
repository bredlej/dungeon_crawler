//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAIN_VIEW_HPP
#define DUNGEON_CRAWLER_MAIN_VIEW_HPP
extern "C" {
#include <raylib.h>
};
#include <core.hpp>
#include <entity_details.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <level_list.hpp>
#include <main_menu.hpp>
#include <map_view.hpp>
#include <memory>

class MainView : public UIView {
public:
    explicit MainView(std::shared_ptr<Core> &core) : UIView(core), _level_list(core), _entity_details{core} {
        _core->dispatcher.sink<RefreshLevels>().connect<&MainView::_refresh_levels>(this);
        _core->dispatcher.sink<LoadLevel>().connect<&MainView::_change_level_name>(this);
        _initialize();
    };
    void render() override;
    void update() override;
    ~MainView() override = default;

private:
    Rectangle _dimension{0, 0, 1024, 768};
    MainMenu _main_menu;
    LevelList _level_list;
    EntityDetails _entity_details;
    void _initialize();
    void _refresh_levels();
    void _change_level_name(const LoadLevel &load_level);
    char _save_as[256] = "level_1.json";
};
#endif//DUNGEON_CRAWLER_MAIN_VIEW_HPP
