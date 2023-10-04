//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_EVENTS_HPP
#define DUNGEON_CRAWLER_EVENTS_HPP
extern "C" {
#include <raylib.h>
}
#include <ecs/components.hpp>
#include <cstdint>
#include <filesystem>
using namespace types;
namespace editor {
    enum class EditMode : uint8_t{
        None,
        Tile,
        Wall
    };
    enum class EditModeTool : uint8_t{
        Select,
        Paint,
        Fill,
        Line
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
        std::vector<components::tiles::MapPosition> positions;
    };
    struct MapPositionHovered {
        components::tiles::MapPosition position;
    };
    struct RectangleSelected {
        Rectangle selection;
    };
    struct ClearSelection {};

    struct TilePlacement {
        int32_t x;
        int32_t y;
    };

    template<typename Component>
    struct PlaceTileComponent {
        Component component;
        std::vector<components::tiles::MapPosition> *positions;
    };
    template<typename Component>
    struct PlaceWallComponent {
        Component component;
        std::vector<std::pair<components::tiles::MapPosition, components::tiles::MapPosition>> *positions;
    };

    struct EntitiesSelected {
        std::vector<std::pair<components::tiles::MapPosition, entt::entity>> entities;
    };

    struct WallSelected {
        std::vector<std::pair<components::tiles::MapPosition, components::tiles::MapPosition>> positions;
    };
    struct WallRemoved {
        components::tiles::MapPosition position1;
        components::tiles::MapPosition position2;
    };
    struct WallAdded {
        WallType type;
        components::tiles::MapPosition position1;
        components::tiles::MapPosition position2;
    };
    struct RemoveAllSelectedEntities {};
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
