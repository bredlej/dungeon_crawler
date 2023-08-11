//
// Created by Patryk Szczypień on 05/04/2023.
//
#include "main_view.hpp"

void MainView::render() noexcept {
    using namespace editor;
    MainMenu::render();
    if (_show_demo) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Begin("Main View", nullptr);
    ImGuiTabBarFlags_ tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("Main View", tab_bar_flags)) {
        if (ImGui::BeginTabItem("Level editor")) {
            _core->registry.ctx().find<MapView>()->render();
            ImGui::SetNextWindowPos(ImVec2(550, 35));
            ImGui::Begin("Main View", nullptr);
            _file_operations.render();
            _level_list.render();
            _edit_mode.render();
            _blueprint.render_and_update();
            _entity_details.render();
            ImGui::End();
            ImGui::EndTabItem();

        }
        if (ImGui::BeginTabItem("Battle editor")) {
            _battle_view.render();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();


}
void MainView::update() noexcept {
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_D)) {
        _core->dispatcher.enqueue<ToggleShowDemo>();
    }
    _core->registry.ctx().find<MapView>()->update();
}

void MainView::_initialize() {
    _core->registry.ctx().emplace<LevelFileName>("new_level.json");
    _core->dispatcher.enqueue<NewLevel>(20, 20);
    _refresh_levels();
}

void MainView::_refresh_levels() {
    _level_list.find_levels("../../assets/levels");
}
void MainView::_toggle_show_demo() {
    _show_demo = !_show_demo;
}
