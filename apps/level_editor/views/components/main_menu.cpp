//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <main_menu.hpp>
void MainMenu::render() {
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("New", "Ctrl+N")){}
            if (ImGui::MenuItem("Open", "Ctrl+O")){}
            if (ImGui::MenuItem("Save", "Ctrl+S")){}
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")){}
            if (ImGui::MenuItem("Exit", "Ctrl+Q")){}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}