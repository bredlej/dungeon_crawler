//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <main_view.hpp>
void MainView::render() {
    using namespace editor;
    MainMenu::render();
    _core->registry.ctx().find<MapView>()->render();
    ImGui::SetNextWindowPos(ImVec2(550, 35));
    //ImGui::SetNextWindowSize(ImVec2(300, 80));

    ImGui::Begin("Main View", nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
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

    ImGui::InputText("", _save_as, 256);
    ImGui::SameLine();
    if (ImGui::Button("Save")) {
        _core->dispatcher.enqueue<SaveLevel>(std::string(_save_as));
    }
    _level_list.render();
    _entity_details.render();

    ImGui::End();
}
void MainView::update() {
}

void MainView::_initialize() {
    _core->registry.ctx().emplace<LevelFileName>("new_level.json");
    _refresh_levels();
}

void MainView::_refresh_levels() {
    _level_list.find_levels("../../assets/levels");
}

void MainView::_change_level_name(const LoadLevel &load_level) {
    std::strcpy(_save_as, load_level.path.filename().string().c_str());
}