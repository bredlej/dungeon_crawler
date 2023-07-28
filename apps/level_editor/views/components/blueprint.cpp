//
// Created by Patryk Szczypień on 14/04/2023.
//
#include "fmt/format.h"
#include <blueprint.hpp>

/**
 * \brief Initializes the Blueprint.
 *
 * This function is responsible for initializing the Blueprint by
 * initializing the tile components and the wall components.
 *
 * \return void
 */
void Blueprint::_initialize() {
    _initialize_tile_components();
    _initialize_wall_components();
}

/**
 * @brief Initializes the tile components for the Blueprint.
 *
 * This function initializes the field and wall component handlers with their respective types.
 * It also populates the list of all available tile component types and sets the selected
 * field component type to the first available type.
 *
 * @see TileComponentType
 * @see WallComponentType
 * @see FloorComponentHandler
 * @see WalkabilityComponentHandler
 * @see EncounterChanceComponentHandler
 * @see WallComponentHandler
 * @see DoorComponentHandler
 *
 * @return void
 */
void Blueprint::_initialize_tile_components() {
    _tile_component_handlers.emplace(TileComponentType::Floor, std::make_unique<FloorComponentHandler>());
    _tile_component_handlers.emplace(TileComponentType::Walkability, std::make_unique<WalkabilityComponentHandler>());
    _tile_component_handlers.emplace(TileComponentType::EncounterChance, std::make_unique<EncounterChanceComponentHandler>());

    _wall_component_handlers.emplace(WallComponentType::Wall, std::make_unique<WallComponentHandler>());
    _wall_component_handlers.emplace(WallComponentType::Door, std::make_unique<DoorComponentHandler>());

    _all_available_tile_component_types.push_back(static_cast<uint8_t>(TileComponentType::Floor));
    _all_available_tile_component_types.push_back(static_cast<uint8_t>(TileComponentType::EncounterChance));
    _all_available_tile_component_types.push_back(static_cast<uint8_t>(TileComponentType::Walkability));
    _selected_field_component_type = _all_available_tile_component_types[0];
}

/**
 * \brief Initializes the wall components.
 *
 * This function initializes the wall components by adding the available wall component types
 * to the '_all_available_wall_component_types' vector and setting the '_selected_wall_component_type'
 * to the first available wall component type.
 *
 * \note This function does not return any value.
 */
void Blueprint::_initialize_wall_components() {
    _all_available_wall_component_types.push_back(static_cast<uint8_t>(WallComponentType::Wall));
    _all_available_wall_component_types.push_back(static_cast<uint8_t>(WallComponentType::Door));
    _selected_wall_component_type = _all_available_wall_component_types[0];
}

/**
 * Sets the selected field component type.
 *
 * @param type The type of the tile component selected.
 * @return void
 */
void Blueprint::_tile_component_selected(TileComponentSelected type) {
    _selected_field_component_type = static_cast<uint8_t>(type.type);
}

/**
 * Add a tile component to the blueprint.
 *
 * This function adds the specified tile component type to the blueprint. It updates the list of added tile
 * component types and removes the type from the list of available tile component types.
 *
 * @param type The tile component being added.
 */
void Blueprint::_tile_component_added(TileComponentAdded type) {
    _added_tile_component_types.push_back(type.type);
    _all_available_tile_component_types.erase(std::remove(_all_available_tile_component_types.begin(), _all_available_tile_component_types.end(), static_cast<uint8_t>(type.type)), _all_available_tile_component_types.end());
    _selected_field_component_type = _all_available_tile_component_types.empty() ? static_cast<uint8_t>(TileComponentType::None) : _all_available_tile_component_types[0];
}

/**
 * @brief Removes a tile component from the Blueprint.
 *
 * This function removes the specified tile component from the Blueprint.
 * It updates the list of added tile component types by removing the specified type.
 * It also adds the removed type to the list of all available tile component types.
 * Finally, it updates the selected field component type to the first available type if the list is not empty.
 *
 * @param type The tile component to be removed.
 */
void Blueprint::_tile_component_removed(TileComponentRemoved type) {
    _added_tile_component_types.erase(std::remove(_added_tile_component_types.begin(), _added_tile_component_types.end(), type.type), _added_tile_component_types.end());
    _all_available_tile_component_types.push_back(static_cast<uint8_t>(type.type));
    _selected_field_component_type = _all_available_tile_component_types.empty() ? static_cast<uint8_t>(TileComponentType::None) : _all_available_tile_component_types[0];
}

/**
 * @brief Updates the selected wall component type.
 *
 * This function updates the selected wall component type by casting the provided
 * type enumeration value to an unsigned 8-bit integer and assigns it to the
 * internal variable _selected_wall_component_type.
 *
 * @param type The type of wall component selected.
 *
 */
void Blueprint::_wall_component_selected(WallComponentSelected type) {
    _selected_wall_component_type = static_cast<uint8_t>(type.type);
}

/**
 * This method is called when a wall component is added to the blueprint.
 * It updates the required data structures to reflect the addition of the wall component.
 *
 * @param type The type of wall component that was added
 */
void Blueprint::_wall_component_added(WallComponentAdded type) {
    _added_wall_component_types.push_back(type.type);
    _all_available_wall_component_types.erase(std::remove(_all_available_wall_component_types.begin(), _all_available_wall_component_types.end(), static_cast<uint8_t>(type.type)), _all_available_wall_component_types.end());
    _selected_wall_component_type = _all_available_wall_component_types.empty() ? static_cast<uint8_t>(WallComponentType::None) : _all_available_wall_component_types[0];
}

/**
 * @brief Remove a wall component from the blueprint.
 *
 * This function removes a wall component of the specified type from the blueprint. It updates the internal state of the blueprint by:
 * - removing the specified type from the list of added wall component types
 * - adding the specified type to the list of all available wall component types
 * - updating the selected wall component type to the first available type, or WallComponentType::None if no types are available
 *
 * @param type The type of wall component to remove.
 */
void Blueprint::_wall_component_removed(WallComponentRemoved type) {
    _added_wall_component_types.erase(std::remove(_added_wall_component_types.begin(), _added_wall_component_types.end(), type.type), _added_wall_component_types.end());
    _all_available_wall_component_types.push_back(static_cast<uint8_t>(type.type));
    _selected_wall_component_type = _all_available_wall_component_types.empty() ? static_cast<uint8_t>(WallComponentType::None) : _all_available_wall_component_types[0];
}

/**
 * @brief Renders the selected tile component.
 *
 * This function renders the tile component of type T from the tile components list.
 * It uses the ComponentRenderer to render the component.
 *
 * @tparam T The type of the tile component to render.
 *
 * @see ComponentRenderer
 */
template <typename T>
void Blueprint::renderTileComponentSelection() {
    _component_renderer.render_component(std::get<T>(_tile_components.components));
}

/**
 * Renders a wall component selection.
 *
 * This function renders the specified wall component by calling the `render_component` function
 * of the component renderer.
 *
 * @tparam T - The type of the wall component to render
 *
 * @note Ensure that the specified wall component type is included in the `_wall_components.components` tuple.
 *
 * @see ComponentRenderer::render_component
 */
template <typename T>
void Blueprint::renderWallComponentSelection() {
    _component_renderer.render_component(std::get<T>(_wall_components.components));
}

/**<enqueueTileComponentPlacement function.
 *
 * This function enqueues a tile component placement for the editor.
 *
 * @param positions A pointer to a vector of MapPosition objects that represent the tile positions.
 * @tparam T The type of the tile component to be placed.
 */
template <typename T>
void Blueprint::enqueueTileComponentPlacement(std::vector<components::tiles::MapPosition> *positions) {
    _core->dispatcher.enqueue<editor::PlaceTileComponent<T>>(std::get<T>(_tile_components.components), positions);
}

/**
 * @brief Enqueues a wall component placement.
 *
 * This function adds a wall component placement task to the dispatcher queue.
 * The task will be processed by the editor::PlaceTileComponent<T> component with the specified positions.
 *
 * @tparam T The type of the wall component.
 * @param positions A pointer to a vector of MapPosition objects specifying the positions to place the wall components.
 *
 * @see editor::PlaceTileComponent<T>
 */
template <typename T>
void Blueprint::enqueueWallComponentPlacement(std::vector<std::pair<components::tiles::MapPosition, components::tiles::MapPosition>> *positions) {
    _core->dispatcher.enqueue<editor::PlaceWallComponent<T>>(std::get<T>(_wall_components.components), positions);
}

/**
 * Renders a combo box widget for selecting a component type.
 *
 * @param available_component_types A vector of available component types.
 * @param selected_component_type The currently selected component type.
 * @param component_type_to_string A map of component types to their corresponding string representations.
 * @param on_component_selected_callback A callback function to be called when a component is selected.
 * @param on_add_button_clicked_callback A callback function to be called when the add button is clicked.
 */
void Blueprint::_render_available_component_combobox(const std::vector<uint8_t>&available_component_types,
                                const uint8_t& selected_component_type,
                                const std::unordered_map<uint8_t, std::string>& component_type_to_string,
                                const std::function<void(uint8_t)>& on_component_selected_callback,
                                const std::function<void(uint8_t)>& on_add_button_clicked_callback) {
    if (available_component_types.empty()) {
        ImGui::Text("No more components available");
        return;
    }
    uint8_t selection = static_cast<uint8_t>(selected_component_type);
    if (ImGui::BeginCombo(component_type_to_string.at(selected_component_type).c_str(),
                          component_type_to_string.at(selected_component_type).c_str())) {
        for (const auto &componentType: available_component_types) {
            const auto &componentName = component_type_to_string.at(componentType);
            if (ImGui::Selectable(componentName.c_str(),
                                  static_cast<uint8_t>(componentType) == selection)) {
                on_component_selected_callback(componentType);
            }
            if (selection == static_cast<uint8_t>(componentType)) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Add")) {
        on_add_button_clicked_callback(selected_component_type);
    }
}

/**
 * @brief Render the properties of a component.
 *
 * This function is used to render the properties of a component in ImGui.
 *
 * @tparam COMPONENT_REMOVE_EVENT The event type used to remove the component.
 * @param index The index of the component.
 * @param component A reference to the component to render.
 * @param type_to_string A reference to a map that converts component types to string names.
 * @param component_handlers A reference to a map of component handlers.
 */
template<typename COMPONENT_REMOVE_EVENT>
void Blueprint::_render_component_properties(int index, auto &component, auto &type_to_string, auto &component_handlers) {
    const auto &component_name = type_to_string[static_cast<uint8_t>(component)];
    if (ImGui::TreeNode(fmt::format("{}##{}", component_name, index++).c_str())) {
        component_handlers[component]->handleComponentSelection(this);
        if (ImGui::Button(fmt::format("Remove##{}", index++).c_str())) {
            _core->dispatcher.enqueue<COMPONENT_REMOVE_EVENT>(component);
        }
        ImGui::TreePop();
    }
}

/**
 * @brief Render and update the Blueprint editor.
 *
 * This function is responsible for rendering and updating the Blueprint editor.
 * It utilizes the ImGui library for creating a graphical user interface.
 * It renders different components and allows the user to interact with them.
 * The editor supports different editing modes such as Tile and Wall modes.
 */
void Blueprint::render_and_update() {
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Blueprint")) {
        auto edit_mode = _core->registry.ctx().find<editor::CurrentEditMode>();
        if (!edit_mode) return;

        switch (edit_mode->edit_mode) {
            case editor::EditMode::Tile: {
                _render_available_component_combobox(
                        _all_available_tile_component_types,
                        _selected_field_component_type,
                        tile_component_type_to_string,
                        [&](uint8_t type) { _core->dispatcher.enqueue<TileComponentSelected>(static_cast<TileComponentType>(type)); },
                        [&](uint8_t type) { _core->dispatcher.enqueue<TileComponentAdded>(static_cast<TileComponentType>(type)); });

                int idx = 0;
                for (const auto &component: _added_tile_component_types) {
                    _render_component_properties<TileComponentRemoved>(idx++, component, tile_component_type_to_string, _tile_component_handlers);
                }
                if (auto *positions = _core->registry.ctx().find<editor::MapPositionSelected>() ) {
                    if (!_added_tile_component_types.empty()) {
                        if (ImGui::Button("Place")) {
                            std::for_each(_added_tile_component_types.begin(), _added_tile_component_types.end(), [&](const auto &component_type) {
                                _tile_component_handlers[component_type]->handleComponentPlacement(this, positions);
                            });
                            _core->dispatcher.update();
                            _core->dispatcher.enqueue<editor::MapPositionSelected>(positions->positions);
                        }
                    }
                }
                break;
            }
            case editor::EditMode::Wall: {
                _render_available_component_combobox(
                        _all_available_wall_component_types,
                        _selected_wall_component_type,
                        wall_component_type_to_string,
                        [&](uint8_t type) { _core->dispatcher.enqueue<WallComponentSelected>(static_cast<WallComponentType>(type)); },
                        [&](uint8_t type) { _core->dispatcher.enqueue<WallComponentAdded>(static_cast<WallComponentType>(type)); });

                int idx = 0;
                for (const auto &component: _added_wall_component_types) {
                    _render_component_properties<WallComponentRemoved>(idx++, component, wall_component_type_to_string, _wall_component_handlers);
                }
                if (auto *wallSelection = _core->registry.ctx().find<editor::WallSelected>() ) {
                    if (!_added_wall_component_types.empty()) {
                        if (ImGui::Button("Place")) {
                            std::for_each(_added_wall_component_types.begin(), _added_wall_component_types.end(), [&](const auto &component_type) {
                                _wall_component_handlers[component_type]->handleComponentPlacement(this, wallSelection);
                            });
                            _core->dispatcher.update();
                            _core->dispatcher.enqueue<editor::WallSelected>(wallSelection->positions);

                        }
                    }
                }
                break;
            }
            case editor::EditMode::None:
                break;
        }

        ImGui::TreePop();
    }
}