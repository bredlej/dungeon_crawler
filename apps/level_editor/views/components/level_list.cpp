//
// Created by Patryk Szczypień on 07/04/2023.
//
#include <level_list.hpp>

void LevelList::render() {
    if (ImGui::CollapsingHeader("Levels"), &_visible, ImGuiTreeNodeFlags_DefaultOpen) {
        for (const auto &level : _levels) {
            if (ImGui::Button(level.filename().c_str())) {
                _core->dispatcher.enqueue<editor::LoadLevel>(level);
            }
        }
    }
}

void LevelList::find_levels(const std::string &path) {
    _levels.clear();
    try {
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_directory()) {
                find_levels(entry.path());
            } else {
                if (entry.path().extension() == ".json") {
                    _levels.push_back(entry.path());
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e) {
        std::printf("%s\n", e.what());
    }
}