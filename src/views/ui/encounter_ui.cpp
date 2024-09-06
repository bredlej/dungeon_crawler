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
            if (ImGui::DragFloat("Speed", &_speed, 0.001f, 0.0f, 1.0f)) {
                //
            }
            if (ImGui::DragFloat("Edge from", &_edge_from, 0.01f, 0.0f, 1.0f)) {
                //
            }
            if (ImGui::DragFloat("Edge to", &_edge_to, 0.01f, 0.0f, 1.0f)) {
                //
            }
            /*static float color_r = _color.r / 255.0f;
            if (ImGui::ColorEdit4("Color", &color_r, ImGuiColorEditFlags_NoInputs)) {
                //
            }*/
        }
        ImGui::End();
    }
    rlImGuiEnd();
}