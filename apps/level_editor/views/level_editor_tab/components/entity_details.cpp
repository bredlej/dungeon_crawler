//
// Created by Patryk Szczypień on 08/04/2023.
//
#include <entity_details.hpp>

void EntityDetails::render() {
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Entity Details")) {
        if (_core->registry.ctx().contains<editor::EntitiesSelected>()) {
            if (ImGui::Button("Remove all")) {
                _core->dispatcher.enqueue<editor::RemoveAllSelectedEntities>();
            }
            int i = 0;
            _core->registry.ctx().erase<editor::MapPositionHovered>();
            for (const auto &[position, entity] : _core->registry.ctx().find<editor::EntitiesSelected>()->entities) {
                bool entity_valid = _core->registry.valid(entity) && entity != entt::null;
                if (entity_valid) {
                    if (ImGui::TreeNode((void *) (intptr_t) i, "%s", fmt::format("({}, {})", position.x, position.y).c_str())) {
                        ImGui::Spacing();
                        if (_core->registry.any_of<components::tiles::TileId>(entity)) {
                            _component_renderer.render_component_of_entity<components::tiles::TileId>(entity);
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
        if (_core->registry.ctx().contains<editor::WallSelected>()) {
            if (ImGui::Button("Remove all")) {
                //_core->dispatcher.enqueue<editor::RemoveAllSelectedWalls>();
            }
            /*int i = 0;
            _core->registry.ctx().erase<editor::MapPositionHovered>();
            for (const auto &[tile1_pos, tile2_pos, entity] : _core->registry.ctx().find<editor::WallSelected>()->positions) {
                bool wall_valid = _core->registry.valid(wall) && wall != entt::null;
                if (wall_valid) {
                    if (ImGui::TreeNode((void *) (intptr_t) i, "%s", fmt::format("({}, {})", position.x, position.y).c_str())) {
                        ImGui::Spacing();
                        if (_core->registry.any_of<components::tiles::Wall>(wall)) {
                            _component_renderer.render_component<components::tiles::Wall>(wall);
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
             */
        }
        ImGui::TreePop();
    }
}
