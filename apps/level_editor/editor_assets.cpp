//
// Created by Patryk Szczypień on 10/04/2023.
//
#include <editor_assets.hpp>
void EditorAssets::load_textures() {
    _textures.emplace(FloorType::RUINS_01, assets::level_editor::tiles::ruins_01);
    _textures.emplace(FloorType::RUINS_02, assets::level_editor::tiles::ruins_02);
}