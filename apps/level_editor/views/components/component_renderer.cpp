//
// Created by Patryk Szczypień on 14/04/2023.
//
#include <component_renderer.hpp>

template<typename MAP, typename TYPE>
static void render_combo(const char* label, MAP map, TYPE &type) {
    if (ImGui::BeginCombo(label, map[type].c_str())) {
        for (auto &map_type: map) {
            bool is_selected = (map_type.first == type);
            if (ImGui::Selectable(map_type.second.c_str(), is_selected)) {
                type = map_type.first;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

template<>
void ComponentRenderer::render_component<components::tiles::Floor>(entt::entity entity) {
    components::tiles::Floor *floor = _core->registry.try_get<components::tiles::Floor>(entity);
    if (floor) {
        const auto label = fmt::format("Floor type##{}", static_cast<int32_t>(entity));
        const auto hidden_label = fmt::format("##floor_type{}", static_cast<int32_t>(entity));
        if (ImGui::TreeNode(label.c_str())) {
            render_combo(hidden_label.c_str(), assets::floor_type_to_name, floor->type);
            ImGui::TreePop();
        }
    }
}

template <>
void ComponentRenderer::render_component<components::tiles::Wall>(components::tiles::Wall &component) {
    render_combo("Wall type", assets::wall_type_to_name, component.type);
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
void ComponentRenderer::render_component<components::tiles::Walkability>(entt::entity entity) {
    components::tiles::Walkability *walkability = _core->registry.try_get<components::tiles::Walkability>(entity);
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
void ComponentRenderer::render_component<components::tiles::Walkability>(components::tiles::Walkability &component) {
    ImGui::Checkbox("Walkable", &component.walkable);
}

template<>
void ComponentRenderer::render_component<components::tiles::TileId>(entt::entity entity) {
    components::tiles::TileId *field = _core->registry.try_get<components::tiles::TileId>(entity);
    if (field) {
        render_component<components::tiles::Floor>(entity);
        render_component<components::tiles::Walkability>(entity);
        render_component<components::values::EncounterChance>(entity);
    }
}

template<>
void ComponentRenderer::render_component<components::tiles::Floor>(components::tiles::Floor &component) {
    render_combo("Floor type", assets::floor_type_to_name, component.type);
}

template <>
void ComponentRenderer::render_component<components::tiles::Door>(components::tiles::Door& component) {
    render_combo("Closed", assets::door_type_to_name, component.typeClosed);
    render_combo("Opened", assets::door_type_to_name, component.typeOpened);
    render_combo("State", assets::door_state_type_to_name, component.state);
}

template<>
void ComponentRenderer::render_component<components::tiles::Door>(entt::entity entity) {
    components::tiles::Door *door = _core->registry.try_get<components::tiles::Door>(entity);
    if (door) {
        render_component<components::tiles::Door>(*(door));
    }
}

template<>
void ComponentRenderer::render_component<components::tiles::Wall>(entt::entity entity) {
    components::tiles::Wall *wall = _core->registry.try_get<components::tiles::Wall>(entity);
    if (wall) {
        render_component<components::tiles::Wall>(*(wall));
    }
}