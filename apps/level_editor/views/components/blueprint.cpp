//
// Created by Patryk Szczypień on 14/04/2023.
//
#include "fmt/format.h"
#include <blueprint.hpp>
void Blueprint::_initialize() {
    _available_field_component_types.push_back(FieldComponentType::Floor);
    _available_field_component_types.push_back(FieldComponentType::EncounterChance);
    _available_field_component_types.push_back(FieldComponentType::Walkability);
    _selected_field_component_type = _available_field_component_types[0];
}
void Blueprint::_field_component_selected(FieldComponentSelected type) {
    _selected_field_component_type = type.type;
}
void Blueprint::_field_component_added(FieldComponentAdded type) {
    _all_field_component_types.push_back(type.type);
    _available_field_component_types.erase(std::remove(_available_field_component_types.begin(), _available_field_component_types.end(), type.type), _available_field_component_types.end());
    _selected_field_component_type = _available_field_component_types.empty() ? FieldComponentType::None : _available_field_component_types[0];
}
void Blueprint::_field_component_removed(FieldComponentRemoved type) {
    std::printf("Removing component: %s", field_component_type_to_string[type.type].c_str());
    _all_field_component_types.erase(std::remove(_all_field_component_types.begin(), _all_field_component_types.end(), type.type), _all_field_component_types.end());
    _available_field_component_types.push_back(type.type);
    _selected_field_component_type = _available_field_component_types.empty() ? FieldComponentType::None : _available_field_component_types[0];
}
void Blueprint::render() {
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Blueprint")) {
        auto edit_mode = _core->registry.ctx().find<editor::CurrentEditMode>();
        if (!edit_mode) return;

        switch (edit_mode->edit_mode) {
            case editor::EditMode::Tile: {
                if (!_available_field_component_types.empty()) {
                    uint8_t selection = static_cast<uint8_t>(_selected_field_component_type);
                    if (ImGui::BeginCombo("Tile", field_component_type_to_string[_selected_field_component_type].c_str())) {
                        for (const auto &field_component_type: _available_field_component_types) {
                            const auto &component_name = field_component_type_to_string[field_component_type];
                            if (ImGui::Selectable(component_name.c_str(), static_cast<uint8_t>(field_component_type) == selection)) {
                                _core->dispatcher.enqueue<FieldComponentSelected>(field_component_type);
                            }
                            if (selection == static_cast<uint8_t>(field_component_type)) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Add")) {
                        _core->dispatcher.enqueue<FieldComponentAdded>(_selected_field_component_type);
                    }
                } else {
                    ImGui::Text("No more components available");
                }

                int idx = 0;
                for (const auto &component: _all_field_component_types) {
                    const auto &component_name = field_component_type_to_string[component];
                    if (ImGui::TreeNode(fmt::format("{}##{}", component_name, idx++).c_str())) {

                        switch (component) {
                            case FieldComponentType::Floor:
                                _component_renderer.render_component(std::get<Floor>(_field_components.components));
                                break;
                            case FieldComponentType::EncounterChance:
                                _component_renderer.render_component(std::get<EncounterChance>(_field_components.components));
                                break;
                            case FieldComponentType::Walkability:
                                _component_renderer.render_component(std::get<Walkability>(_field_components.components));
                                break;
                            case FieldComponentType::None:
                                break;
                        }
                        if (ImGui::Button(fmt::format("Remove##{}", idx++).c_str())) {
                            _core->dispatcher.enqueue<FieldComponentRemoved>(component);
                        }

                        ImGui::TreePop();
                    }
                }
                break;
            }
            case editor::EditMode::Wall:
                break;
            case editor::EditMode::None:
                break;
        }
        if (auto *positions = _core->registry.ctx().find<editor::MapPositionSelected>()) {
            if (!_all_field_component_types.empty()) {
                if (ImGui::Button("Place")) {
                    std::for_each(_all_field_component_types.begin(), _all_field_component_types.end(), [&](const auto &component_type) {
                      switch (component_type) {
                          case FieldComponentType::Floor:
                              _core->dispatcher.enqueue<editor::PlaceComponent<Floor>>(std::get<Floor>(_field_components.components), &positions->positions);
                              break;
                          case FieldComponentType::EncounterChance:
                              _core->dispatcher.enqueue<editor::PlaceComponent<EncounterChance>>(std::get<EncounterChance>(_field_components.components), &positions->positions);
                              break;
                          case FieldComponentType::Walkability:
                              _core->dispatcher.enqueue<editor::PlaceComponent<Walkability>>(std::get<Walkability>(_field_components.components), &positions->positions);
                              break;
                          case FieldComponentType::None:
                              break;
                      }
                    });
                    _core->dispatcher.update();
                    _core->dispatcher.enqueue<editor::MapPositionSelected>(positions->positions);
                }
            }
        }

        ImGui::TreePop();
    }
}