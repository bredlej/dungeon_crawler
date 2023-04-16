//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_EVENTS_HPP
#define DUNGEON_CRAWLER_EVENTS_HPP
#include <filesystem>
#include <components.hpp>
#include <cstdint>
namespace editor {
    enum class EditMode : uint8_t{
        None,
        Tile,
        Wall
    };
    enum class EditModeTool {
        Select,
        Paint,
        Fill
    };
    struct CurrentEditMode {
        EditMode edit_mode;
    };
    struct ChangeEditMode {
        EditMode edit_mode;
    };
    struct CurrentEditModeTool {
        EditModeTool edit_mode_tool;
    };
    struct ChangeEditModeTool {
        EditModeTool edit_mode_tool;
    };
    struct RefreshLevels {};
    struct ShowError {
        std::string message;
    };
    struct MapPositionSelected {
        std::vector<components::fields::MapPosition> positions;
    };
    struct MapPositionHovered {
        components::fields::MapPosition position;
    };
    struct RectangleSelected {
        Rectangle selection;
    };
    struct ClearSelection {};

    template<typename Component>
    struct PlaceComponent{
        Component component;
    };

    struct EntitiesSelected {
        std::vector<std::pair<components::fields::MapPosition, entt::entity>> entities;
    };
    struct ToggleShowDemo {};
    struct LevelFileName {
        std::string name;
    };
    struct NewLevel {
        int width;
        int height;
    };
    struct LoadLevel {
        std::filesystem::path path;
    };
    struct SaveLevel {
        std::string path;
    };
    struct EntitySelected {
        entt::entity entity;
    };
}// namespace editor
#endif//DUNGEON_CRAWLER_EVENTS_HPP
