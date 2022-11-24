//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>
void DungeonView::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    BeginTextureMode(_render_texture);
    if (auto *assets = _core->get_assets()) {
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F01][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F04][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F06][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F07][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F08][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F09][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F10][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F11][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F12][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F13][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F15][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F16][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F17][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F18][FloorType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F19][FloorType::NORMAL].get(), 0, 0, WHITE);

        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F01][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F04][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F06][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F07][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F08][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F09][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F10][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F11][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F12][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F13][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F15][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F16][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F17][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F18][CeilingType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F19][CeilingType::NORMAL].get(), 0, 0, WHITE);

        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F02][WallType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F03][WallType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F05][WallType::NORMAL].get(), 0, 0, WHITE);
        DrawTexture(assets->_textures._tiles[assets::dungeon_view::POVField::F14][WallType::NORMAL].get(), 0, 0, WHITE);
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

