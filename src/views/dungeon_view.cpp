//
// Created by geoco on 19.11.2022.
//

#include "dungeon_view.hpp"
void DungeonView::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    BeginTextureMode(_render_texture);
    if (auto *assets = _core->get_assets()) {
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F01][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F04][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F06][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F07][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F08][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F09][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F10][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F11][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F12][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F13][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F15][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F16][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F17][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F18][assets::FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F19][assets::FloorType::NORMAL].get(), 0, 0, WHITE);

        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F01][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F04][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F06][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F07][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F08][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F09][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F10][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F11][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F12][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F13][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F15][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F16][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F17][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F18][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F19][assets::CeilingType::NORMAL].get(), 0, 0, WHITE);

        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][assets::WallType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][assets::WallType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][assets::WallType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][assets::WallType::NORMAL].get(), 0, 0, WHITE);


    }
    EndTextureMode();
    DrawTexturePro(_render_texture.texture,
                   Rectangle{0.0f, 0.0f, (float) _render_texture.texture.width, (float) -_render_texture.texture.height},
                   Rectangle{15.0f, 15.0f, static_cast<float>(640.0f), static_cast<float>(480.0f)},
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
    EndDrawing();
}
void DungeonView::update() {
}

