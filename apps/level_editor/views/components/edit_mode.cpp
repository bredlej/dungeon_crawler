//
// Created by Patryk Szczypień on 10/04/2023.
//
#include <edit_mode.hpp>
template<>
void EditModeSelector::_render_edit_mode_tools<editor::EditMode::Tile>() {
    using namespace editor;
    static int choice = 0;
    if (ImGui::RadioButton("Select", choice == 0)) {
        choice = 0;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Paint", choice == 1)) {
        choice = 1;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Fill", choice == 2)) {
        choice = 2;
    }
}

void EditModeSelector::render() {
    using namespace editor;
    bool open = true;
    if (ImGui::CollapsingHeader("Edit Mode", &_visible, ImGuiTreeNodeFlags_DefaultOpen)) {
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
        if (edit_mode == EditMode::Tile) {
            _render_edit_mode_tools<EditMode::Tile>();
        }
    }
}
