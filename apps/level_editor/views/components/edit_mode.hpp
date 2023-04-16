//
// Created by Patryk Szczypień on 10/04/2023.
//

#ifndef DUNGEON_CRAWLER_EDIT_MODE_HPP
#define DUNGEON_CRAWLER_EDIT_MODE_HPP
#include <core.hpp>
#include <events.hpp>
#include <memory>

class EditModeSelector {
public:
    explicit EditModeSelector(std::shared_ptr<Core> core) : _core{core} {
        _core->dispatcher.sink<editor::ChangeEditMode>().connect<&EditModeSelector::set_edit_mode>(this);
        _core->dispatcher.sink<editor::ChangeEditModeTool>().connect<&EditModeSelector::set_edit_mode_tool>(this);
        _core->registry.ctx().emplace<editor::CurrentEditMode>(editor::EditMode::None);
        _core->registry.ctx().emplace<editor::CurrentEditModeTool>(editor::EditModeTool::Select);
    };
    EditModeSelector(const EditModeSelector &) = delete;
    EditModeSelector(EditModeSelector &&) = default;
    EditModeSelector &operator=(const EditModeSelector &) = delete;
    EditModeSelector &operator=(EditModeSelector &&) = delete;
    ~EditModeSelector() = default;
    void render();
    void update();
    void set_edit_mode(editor::ChangeEditMode event);
    void set_edit_mode_tool(editor::ChangeEditModeTool event);

private:
    bool _visible{true};
    std::shared_ptr<Core> _core;
    template<editor::EditMode T>
    void _render_edit_mode_tools();
};
#endif//DUNGEON_CRAWLER_EDIT_MODE_HPP
