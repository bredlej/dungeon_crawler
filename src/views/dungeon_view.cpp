//
// Created by geoco on 19.11.2022.
//

#include "dungeon_view.hpp"
void DungeonView::render() {
    BeginDrawing();
    ClearBackground(BLACK);
    if (auto *assets = _core->get_assets()) {
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F01][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F04][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F06][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F07][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F08][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F09][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F10][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F11][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F12][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F13][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F15][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F16][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F17][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F18][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F19][assets::FloorType::NORMAL].get(), 0, 0, RAYWHITE);
    }
    EndDrawing();
}
void DungeonView::update() {
}
