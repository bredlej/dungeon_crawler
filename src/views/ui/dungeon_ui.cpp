//
// Created by Patryk Szczypień on 20/03/2023.
//
#include <views/ui/dungeon_ui.hpp>

void DungeonUI::render() {
    rlImGuiBegin();
    if (_show_demo) {
        ImGui::ShowDemoWindow(&_show_demo);
    }
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        const float PAD = 45.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = work_pos.x + (work_size.x / 2.0f) + PAD*2;
        window_pos.y = work_pos.y + work_size.y - PAD;
        window_pos_pivot.x = 1.0f;
        window_pos_pivot.y = 1.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    ImVec2 button_sz(60, 60);
    if (ImGui::Begin("Movement", &_bottom_window_visible, window_flags)) {
        if (ImGui::Button("LT", button_sz)) {
            _core->_dispatcher.enqueue(events::dungeon::TurnLeft {});
        };
        ImGui::SameLine();
        if (ImGui::Button("UP", button_sz)) {
            _core->_dispatcher.enqueue(events::dungeon::MoveForward {});
        }
        ImGui::SameLine();
        if (ImGui::Button("RT", button_sz)) {
            _core->_dispatcher.enqueue(events::dungeon::TurnRight {});
        }
        if (ImGui::Button("LEFT", button_sz)) {
            _core->_dispatcher.enqueue(events::dungeon::MoveLeft {});
        }
        ImGui::SameLine();
        if (ImGui::Button("DOWN", button_sz)) {
            _core->_dispatcher.enqueue(events::dungeon::MoveBack {});
        }
        ImGui::SameLine();
        if (ImGui::Button("RIGHT",button_sz)) {
            _core->_dispatcher.enqueue(events::dungeon::MoveRight {});
        }
    }
    ImGui::End();
    rlImGuiEnd();
}

void DungeonUI::_toggle_demo() {
    _show_demo = !_show_demo;
}
void DungeonUI::_initialize() {
    _core->_dispatcher.sink<events::ui::ToggleShowDemo>().connect<&DungeonUI::_toggle_demo>(this);
}
