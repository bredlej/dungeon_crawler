//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_EVENTS_HPP
#define DUNGEON_CRAWLER_EVENTS_HPP
#include <filesystem>
namespace editor {
    enum class EditMode {
        None,
        Tile,
        Wall
    };
    struct CurrentEditMode {
        EditMode edit_mode;
    };
    struct ChangeEditMode {
        EditMode edit_mode;
    };
    struct RefreshLevels {};
    struct ShowError {
        std::string message;
    };
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
