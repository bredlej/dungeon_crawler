//
// Created by Patryk Szczypień on 10/04/2023.
//
#include <edit_mode.hpp>
template<>
void EditModeSelector::_render_edit_mode_tools<editor::EditMode::Tile>() {
    using namespace editor;
    static int choice = 0;
    auto edit_mode_tool = _core->registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool;
    if (ImGui::RadioButton("Select", edit_mode_tool == EditModeTool::Select)) {
        _core->dispatcher.enqueue<ChangeEditModeTool>(EditModeTool::Select);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Paint", edit_mode_tool == EditModeTool::Paint)) {
        _core->dispatcher.enqueue<ChangeEditModeTool>(EditModeTool::Paint);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Fill", edit_mode_tool == EditModeTool::Fill)) {
        _core->dispatcher.enqueue<ChangeEditModeTool>(EditModeTool::Fill);
    }
}

template<>
void EditModeSelector::_render_edit_mode_tools<editor::EditMode::Wall>() {
    using namespace editor;
    static int choice = 0;
    auto edit_mode_tool = _core->registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool;
    if (ImGui::RadioButton("Select", edit_mode_tool == EditModeTool::Select)) {
        _core->dispatcher.enqueue<ChangeEditModeTool>(EditModeTool::Select);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Paint", edit_mode_tool == EditModeTool::Paint)) {
        _core->dispatcher.enqueue<ChangeEditModeTool>(EditModeTool::Paint);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Line", edit_mode_tool == EditModeTool::Line)) {
        _core->dispatcher.enqueue<ChangeEditModeTool>(EditModeTool::Line);
    }
}

void EditModeSelector::render() {
    using namespace editor;
    bool open = true;
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Edit Mode")) {
        auto edit_mode = _core->registry.ctx().find<CurrentEditMode>()->edit_mode;
        if (ImGui::RadioButton("None", edit_mode == EditMode::None)) {
            _core->dispatcher.enqueue<ChangeEditMode>(EditMode::None);
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Tile", edit_mode == EditMode::Tile)) {
            _core->dispatcher.enqueue<ChangeEditMode>(EditMode::Tile);
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Wall", edit_mode == EditMode::Wall)) {
            _core->dispatcher.enqueue<ChangeEditMode>(EditMode::Wall);
        }
        switch (edit_mode) {
            case EditMode::None:
                break;
            case EditMode::Tile:
                _render_edit_mode_tools<EditMode::Tile>();
                break;
            case EditMode::Wall:
                _render_edit_mode_tools<EditMode::Wall>();
                break;
        }
        ImGui::TreePop();
    }
}
void EditModeSelector::set_edit_mode(editor::ChangeEditMode event) {
    _core->dispatcher.enqueue<editor::ClearSelection>();
    _core->registry.ctx().erase<editor::CurrentEditMode>();
    _core->registry.ctx().emplace<editor::CurrentEditMode>(event.edit_mode);
    _core->dispatcher.enqueue<editor::ChangeEditModeTool>(editor::EditModeTool::Select);
    _core->dispatcher.enqueue<editor::ClearSelection>();
}
void EditModeSelector::set_edit_mode_tool(editor::ChangeEditModeTool event) {
    _core->registry.ctx().erase<editor::CurrentEditModeTool>();
    _core->registry.ctx().emplace<editor::CurrentEditModeTool>(event.edit_mode_tool);
}
