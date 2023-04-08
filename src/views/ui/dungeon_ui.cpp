//
// Created by Patryk Szczypień on 20/03/2023.
//
#include <views/ui/dungeon_ui.hpp>

void DungeonUI::render() {
    rlImGuiBegin();
    if (_show_demo) {
        ImGui::ShowDemoWindow(&_show_demo);
    }
    _render_movement_buttons();
    _render_encounter_chance();
    _core->game_log.render_no_border("GameLog", &_show_demo);
    rlImGuiEnd();
}

void DungeonUI::_toggle_demo() {
    _show_demo = !_show_demo;
}
void DungeonUI::_initialize() {
    _core->dispatcher.sink<events::ui::ToggleShowDemo>().connect<&DungeonUI::_toggle_demo>(this);
}

void DungeonUI::_render_movement_buttons() {
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
            _core->dispatcher.enqueue(events::dungeon::TurnLeft {});
            _core->game_log.message("Turn left.\n");
        };
        ImGui::SameLine();
        if (ImGui::Button("UP", button_sz)) {
            _core->dispatcher.enqueue(events::dungeon::MoveForward {});
        }
        ImGui::SameLine();
        if (ImGui::Button("RT", button_sz)) {
            _core->dispatcher.enqueue(events::dungeon::TurnRight {});
        }
        if (ImGui::Button("LEFT", button_sz)) {
            _core->dispatcher.enqueue(events::dungeon::MoveLeft {});
        }
        ImGui::SameLine();
        if (ImGui::Button("DOWN", button_sz)) {
            _core->dispatcher.enqueue(events::dungeon::MoveBack {});
        }
        ImGui::SameLine();
        if (ImGui::Button("RIGHT",button_sz)) {
            _core->dispatcher.enqueue(events::dungeon::MoveRight {});
        }
    }
    ImGui::End();
}
void DungeonUI::_render_encounter_chance() {
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize |  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
    ImVec2 work_pos = ImGui::GetMainViewport()->WorkPos;// Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = ImGui::GetMainViewport()->WorkSize;
    ImVec2 window_pos, window_pos_pivot;

    const float PAD = 15.0f;

    window_pos.x = work_size.x / 3 + (3*PAD);
    window_pos.y = work_pos.y + PAD;
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 1.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);

    ImGui::SetNextWindowBgAlpha(0.25f);// Transparent background
    if (!ImGui::Begin("Encounter", &_encounter_chance_window_visible, window_flags)) {
        ImGui::End();
        return;
    }
    auto encounter_chance = _core->registry.ctx().find<components::values::EncounterChance>()->chance;
    if (encounter_chance < 0.6) {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(static_cast<float>(palette::green.r)/255.0f, static_cast<float>(palette::green.g)/255.0f, static_cast<float>(palette::green.b)/255.0f, 1));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(static_cast<float>(palette::red.r)/255.0f, static_cast<float>(palette::red.g)/255.0f, static_cast<float>(palette::red.b)/255.0f, 1));
    }
    ImGui::ProgressBar(encounter_chance, ImVec2(0,0));
    ImGui::PopStyleColor(1);
    ImGui::End();

}
