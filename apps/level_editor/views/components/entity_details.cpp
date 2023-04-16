//
// Created by Patryk Szczypień on 08/04/2023.
//
#include <entity_details.hpp>

void EntityDetails::render() {
    if (ImGui::TreeNode("Entity Details")) {
        if (_core->registry.ctx().contains<editor::EntitiesSelected>()) {
            int i = 0;
            _core->registry.ctx().erase<editor::MapPositionHovered>();
            for (const auto &[position, entity] : _core->registry.ctx().find<editor::EntitiesSelected>()->entities) {
                bool entity_valid = _core->registry.valid(entity) && entity != entt::null;
                if (entity_valid) {
                    if (ImGui::TreeNode((void *) (intptr_t) i, fmt::format("({}, {})", position.x, position.y).c_str())) {
                        ImGui::Spacing();
                        if (_core->registry.any_of<components::fields::Field>(entity)) {
                            _component_renderer.render_component<components::fields::Field>(entity);
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::IsItemHovered()) {
                        _core->registry.ctx().emplace<editor::MapPositionHovered>(position);
                    }
                }
                else {
                    ImGui::CollapsingHeader(fmt::format("({}, {}) - empty -", position.x, position.y).c_str(), ImGuiTreeNodeFlags_Leaf);
                    if (ImGui::IsItemHovered()) {
                        _core->registry.ctx().emplace<editor::MapPositionHovered>(position);
                    }
                }
                i++;
            }
        }
        ImGui::TreePop();
    }
}
