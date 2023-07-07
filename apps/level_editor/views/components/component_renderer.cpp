//
// Created by Patryk Szczypień on 14/04/2023.
//
#include <component_renderer.hpp>

template<>
void ComponentRenderer::render_component<components::fields::Floor>(entt::entity entity) {
    components::fields::Floor *floor = _core->registry.try_get<components::fields::Floor>(entity);
    if (floor) {
        const auto label = fmt::format("Floor type##{}", static_cast<int32_t>(entity));
        const auto hidden_label = fmt::format("##floor_type{}", static_cast<int32_t>(entity));
        if (ImGui::TreeNode(label.c_str())) {
            if (ImGui::BeginCombo(hidden_label.c_str(), assets::floor_type_to_name[floor->type].c_str())) {
                for (auto &floor_type: assets::floor_type_to_name) {
                    bool is_selected = (floor_type.first == floor->type);
                    if (ImGui::Selectable(floor_type.second.c_str(), is_selected)) {
                        floor->type = floor_type.first;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::TreePop();
        }
    }
}

template<>
void ComponentRenderer::render_component<components::fields::Floor>(components::fields::Floor &component) {
    if (ImGui::BeginCombo("Floor type", assets::floor_type_to_name[component.type].c_str())) {
        for (auto &floor_type: assets::floor_type_to_name) {
            bool is_selected = (floor_type.first == component.type);
            if (ImGui::Selectable(floor_type.second.c_str(), is_selected)) {
                component.type = floor_type.first;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

template<>
void ComponentRenderer::render_component<components::values::EncounterChance>(entt::entity entity) {
    components::values::EncounterChance *encounter_chance = _core->registry.try_get<components::values::EncounterChance>(entity);
    if (encounter_chance) {
        const auto label = fmt::format("Encounter chance##{}", static_cast<int32_t>(entity));
        const auto hidden_label = fmt::format("##encounter{}", static_cast<int32_t>(entity));
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(label.c_str())) {
            ImGui::InputFloat(hidden_label.c_str(), &encounter_chance->chance);
            ImGui::TreePop();
        }
    }
}

template<>
void ComponentRenderer::render_component<components::values::EncounterChance>(components::values::EncounterChance &component) {
    ImGui::InputFloat("Encounter chance", &component.chance);
}

template<>
void ComponentRenderer::render_component<components::fields::Walkability>(entt::entity entity) {
    components::fields::Walkability *walkability = _core->registry.try_get<components::fields::Walkability>(entity);
    if (walkability) {
        const auto label = fmt::format("Walkable##{}", static_cast<int32_t>(entity));
        const auto hidden_label = fmt::format("##walkable{}", static_cast<int32_t>(entity));
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode(label.c_str())) {
            ImGui::Checkbox(hidden_label.c_str(), &walkability->walkable);
            ImGui::TreePop();
        }
    }
}

template<>
void ComponentRenderer::render_component<components::fields::Walkability>(components::fields::Walkability &component) {
    ImGui::Checkbox("Walkable", &component.walkable);
}

template<>
void ComponentRenderer::render_component<components::fields::Field>(entt::entity entity) {
    components::fields::Field *field = _core->registry.try_get<components::fields::Field>(entity);
    if (field) {
        render_component<components::fields::Floor>(entity);
        render_component<components::fields::Walkability>(entity);
        render_component<components::values::EncounterChance>(entity);
    }
}