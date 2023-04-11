//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAIN_VIEW_HPP
#define DUNGEON_CRAWLER_MAIN_VIEW_HPP
extern "C" {
#include <raylib.h>
};
#include <core.hpp>
#include <edit_mode.hpp>
#include <entity_details.hpp>
#include <file_operations.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <level_list.hpp>
#include <main_menu.hpp>
#include <map_view.hpp>
#include <memory>

class MainView : public UIView<MainView> {
public:
    explicit MainView(std::shared_ptr<Core> &core) : UIView(core),
                                                     _level_list(core),
                                                     _entity_details{core},
                                                     _edit_mode{core},
                                                     _file_operations{core} {
        _core->dispatcher.sink<RefreshLevels>().connect<&MainView::_refresh_levels>(this);
        _initialize();
    };
    void render() noexcept override;
    void update() noexcept override;
    ~MainView() override = default;

private:
    Rectangle _dimension{0, 0, 1024, 768};
    MainMenu _main_menu;
    FileOperations _file_operations;
    LevelList _level_list;
    EntityDetails _entity_details;
    EditModeSelector _edit_mode;
    void _initialize();
    void _refresh_levels();
};
#endif//DUNGEON_CRAWLER_MAIN_VIEW_HPP
