//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAIN_VIEW_HPP
#define DUNGEON_CRAWLER_MAIN_VIEW_HPP
extern "C" {
#include "raylib-4.0.0/src/raylib.h"
};
#include "engine/core.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "views/battle_editor_tab/battle_view.hpp"
#include "views/level_editor_tab/components/blueprint.hpp"
#include "views/level_editor_tab/components/edit_mode.hpp"
#include "views/level_editor_tab/components/entity_details.hpp"
#include "views/level_editor_tab/components/file_operations.hpp"
#include "views/level_editor_tab/components/level_list.hpp"
#include "views/level_editor_tab/components/main_menu.hpp"
#include "views/level_editor_tab/editor_events.hpp"
#include "views/level_editor_tab/map_view.hpp"
#include <memory>

class MainView : public UIView<MainView> {
public:
    explicit MainView(std::shared_ptr<Core> &core) : UIView(core),
                                                     _level_list(core),
                                                     _entity_details{core},
                                                     _edit_mode{core},
                                                     _file_operations{core},
                                                     _blueprint{core},
                                                     _battle_view{core} {
        _core->dispatcher.sink<RefreshLevels>().connect<&MainView::_refresh_levels>(this);
        _core->dispatcher.sink<ToggleShowDemo>().connect<&MainView::_toggle_show_demo>(this);
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
    Blueprint _blueprint;
    BattleView _battle_view;
    void _initialize();
    void _refresh_levels();
    bool _show_demo{false};
    void _toggle_show_demo();
};
#endif//DUNGEON_CRAWLER_MAIN_VIEW_HPP
