//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <main_view.hpp>
void MainView::render() {
    MainMenu::render();
    _core->registry.ctx().find<MapView>()->render();
    ImGui::SetNextWindowPos(ImVec2(550, 35));
    ImGui::SetNextWindowSize(ImVec2(300, 60));

    ImGui::Begin("Main View", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Edit mode");
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


    ImGui::End();
}
void MainView::update() {
}
