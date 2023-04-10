//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <main_view.hpp>
void MainView::render() {
    using namespace editor;
    MainMenu::render();
    _core->registry.ctx().find<MapView>()->render();
    ImGui::SetNextWindowPos(ImVec2(550, 35));
    ImGui::Begin("Main View", nullptr,  ImGuiWindowFlags_NoTitleBar);
    _file_operations.render();
    _edit_mode.render();
    _level_list.render();
    _entity_details.render();

    ImGui::End();
}
void MainView::update() {
}

void MainView::_initialize() {
    _core->registry.ctx().emplace<LevelFileName>("new_level.json");
    _core->dispatcher.enqueue<NewLevel>(20, 20);
    _refresh_levels();
}

void MainView::_refresh_levels() {
    _level_list.find_levels("../../assets/levels");
}
