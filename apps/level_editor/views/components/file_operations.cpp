//
// Created by Patryk Szczypień on 10/04/2023.
//
#include <file_operations.hpp>
void FileOperations::render() {
    using namespace editor;
    ImGui::InputText("", _save_as, 256);
    ImGui::SameLine();
    if (ImGui::Button("Save")) {
        _core->dispatcher.enqueue<SaveLevel>(std::string(_save_as));
    }
    if (ImGui::TreeNode("File Operations")) {
        if (ImGui::Button("New")) {
            ImGui::OpenPopup("New Level");
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        if (ImGui::BeginPopupModal("New Level", &_visible, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Enter the size of the new level");
            ImGui::Separator();
            static int width = 20;
            static int height = 20;
            ImGui::InputInt("Width", &width);
            ImGui::InputInt("Height", &height);
            if (ImGui::Button("Create", ImVec2(120, 0))) {
                _core->dispatcher.enqueue<editor::NewLevel>(width, height);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }

}

void FileOperations::_change_level_name(const editor::LoadLevel &load_level) {
    std::string level_name = load_level.path.filename().string();
    std::strcpy(_save_as, level_name.c_str());
}