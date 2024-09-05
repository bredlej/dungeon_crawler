//
// Created by Patryk Szczypień on 05/09/2024.
//
#include <views/ui/encounter_ui.hpp>

void EncounterUI::_initialize() {

}
void EncounterUI::render() {
    rlImGuiBegin();
    {
        if (ImGui::Begin("Encounter", &_bottom_window_visible)) {
            if (ImGui::DragFloat("Back row x", &_back_row_x, 1.0f, 0.0f, 320.0f)) {
                //
            }
            if (ImGui::DragFloat("Back row y", &_back_row_y, 1.0f, 0.0f, 240.0f)) {
                //
            }
            if (ImGui::DragFloat("Front row x", &_front_row_x, 1.0f, 0.0f, 320.0f)) {
                //
            }
            if (ImGui::DragFloat("Front row y", &_front_row_y, 1.0f, 0.0f, 240.0f)) {
                //
            }
            if (ImGui::DragFloat("Offset", &_offset, 1.0f, 0.0f, 240.0f)) {
                //
            }
        }
        ImGui::End();
    }
    rlImGuiEnd();
}