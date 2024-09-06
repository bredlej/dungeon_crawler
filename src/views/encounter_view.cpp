//
// Created by Patryk Szczypień on 16/08/2024.
//

#include <fmt/format.h>
#include <views/encounter_view.hpp>
using namespace components::battle;

void EncounterView::_initialize() noexcept {
    for (int i = 0; i < RENDER_TEXTURE_WIDTH * RENDER_TEXTURE_HEIGHT; i++) {
        _empty_canvas.push_back(BLANK);
    }
    _core->scheduler.attach([this](auto delta, void *, auto succeed, auto fail){
        _timer+=delta * _ui.speed();
        if (_timer > 360.0f) {
            _timer = 0.0f;
        }
    });
}

void EncounterView::render() noexcept {

    BeginDrawing();
    ClearBackground(BLACK);
    {
        BeginTextureMode(_canvas);
        {
            _clear_canvas();
            _render_enemies();
        }
        EndTextureMode();

        _battle_director->update();
    }
    static auto POV_DIMENSION_FULLSCREEN = Rectangle{0, 0, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    _render_texture(_canvas.texture, POV_DIMENSION_FULLSCREEN);
    _render_mouseover();
    _render_texture(_canvas.texture, POV_DIMENSION_FULLSCREEN);
    _ui.render();
    EndDrawing();
}

void EncounterView::update() noexcept {
    if (IsKeyReleased(KEY_DOWN)) {
        _core->dispatcher.enqueue(events::dungeon::EndEncounter{});
    }
}

void EncounterView::_render_mouseover() noexcept {
    Image image = LoadImageFromTexture(_canvas.texture);
    auto *colors = LoadImageColors(image);
    int window_width = POV_DIMENSION.width;
    int window_height = POV_DIMENSION.height;
    int image_width = 320;
    int image_height = 240;

    float scale_x = static_cast<float>(image_width) / static_cast<float>(window_width);
    float scale_y = static_cast<float>(image_height) / static_cast<float>(window_height);

    int image_mouse_x = static_cast<int>(GetMouseX() * scale_x);
    int image_mouse_y = static_cast<int>(GetMouseY() * scale_y);

    int index = ((image_height - image_mouse_y - 1) * image.width) + image_mouse_x;
    Color pixel = colors[index];

    auto back_row_y = _ui.back_row_y();
    auto front_row_y = _ui.front_row_y();
    auto back_row_start_x = _ui.back_row_x();
    auto front_row_start_x = _ui.front_row_x();
    auto offset = _ui.offset();

    if (auto *battle_ctx = _core->registry.ctx().find<config::BattleContext>()) {
        if (auto *enemy_config = _core->registry.try_get<config::EnemyConfig>(battle_ctx->entity)) {
            if (auto *two_rows = _core->registry.try_get<placement::TwoRows<3>>(enemy_config->entity)) {
                if (pixel.a == 255) {
                    auto i = 0;
                    entt::entity back_row_selected = _get_entity_at_point(i, two_rows->rows[placement::Row::BACK_ROW], back_row_start_x, back_row_y, offset, image_mouse_x, image_mouse_y);
                    i = 0;
                    entt::entity front_row_selected = _get_entity_at_point(i, two_rows->rows[placement::Row::FRONT_ROW], front_row_start_x, front_row_y, offset, image_mouse_x, image_mouse_y);

                    if (back_row_selected != entt::null && front_row_selected == entt::null) {
                        if (auto *name = _core->registry.try_get<components::general::Name>(back_row_selected)) {
                            DrawTextEx(_core->get_assets()->fonts.font, name->name.c_str(), Vector2{10, 660}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
                           _selected_enemy = back_row_selected;
                        }
                    } else if (front_row_selected != entt::null) {
                        if (auto *name = _core->registry.try_get<components::general::Name>(front_row_selected)) {
                            DrawTextEx(_core->get_assets()->fonts.font, name->name.c_str(), Vector2{10, 660}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
                            _selected_enemy = front_row_selected;
                        }
                    }
                    else {
                        _selected_enemy = entt::null;
                    }
                    //
                }
            }
        }
    }
    // draw mouse coordinates
    DrawTextEx(_core->get_assets()->fonts.font, fmt::format("X: {} Y: {}", image_mouse_x, image_mouse_y).c_str(), Vector2{10, 680}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, fmt::format("R: {:.2f} G: {:.2f} B: {:.2f} A: {:.2f}", (pixel.r / 255.0f), (pixel.g / 255.0f), (pixel.b / 255.0f), (pixel.a / 255.0f)).c_str(), Vector2{10, 700}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, fmt::format("Timer: {:.2f}", _timer).c_str(), Vector2{10, 720}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    UnloadImageColors(colors);
}

void EncounterView::_render_enemies() noexcept {
    auto back_row_y = _ui.back_row_y();
    auto front_row_y = _ui.front_row_y();
    auto back_row_start_x = _ui.back_row_x();
    auto front_row_start_x = _ui.front_row_x();
    auto offset = _ui.offset();
    if (auto *battle_ctx = _core->registry.ctx().find<config::BattleContext>()) {
        if (auto *enemy_config = _core->registry.try_get<config::EnemyConfig>(battle_ctx->entity)) {
            if (auto *two_rows = _core->registry.try_get<placement::TwoRows<3>>(enemy_config->entity)) {
                auto i = 0;
                _render_enemy_slot(i, two_rows->rows[placement::Row::BACK_ROW], back_row_start_x, back_row_y, offset);
                i = 0;
                _render_enemy_slot(i, two_rows->rows[placement::Row::FRONT_ROW], front_row_start_x, front_row_y, offset);
            }
        }
    }
}

void EncounterView::_render_enemy(entt::entity entity, int x, int y) noexcept {
    if (auto *enemy_type = _core->registry.try_get<enemies::EnemyType>(entity)) {
        const auto *enemy_texture = &_core->get_assets()->_textures._enemies[enemy_type->value];
        DrawTextureRec(enemy_texture->get(), {0, 0, static_cast<float>(enemy_texture->get().width), static_cast<float>(enemy_texture->get().height)}, {static_cast<float>(x - (enemy_texture->get().width / 2)), static_cast<float>(y - enemy_texture->get().height)}, WHITE);
        if (_selected_enemy == entity) {
            BeginBlendMode(BLEND_MULTIPLIED);
            BeginShaderMode(_fill_shader);
            float edge_from = _ui.edge_from();
            float edge_to = _ui.edge_to();
            DrawTextureRec(enemy_texture->get(), {0, 0, static_cast<float>(enemy_texture->get().width), static_cast<float>(enemy_texture->get().height)}, {static_cast<float>(x - (enemy_texture->get().width / 2)), static_cast<float>(y - enemy_texture->get().height)}, WHITE);
            EndShaderMode();
            BeginShaderMode(_outline_shader);
            SetShaderValue(_outline_shader, GetShaderLocation(_outline_shader, "edge_from"), &edge_from, SHADER_UNIFORM_FLOAT);
            SetShaderValue(_outline_shader, GetShaderLocation(_outline_shader, "edge_to"), &edge_to, SHADER_UNIFORM_FLOAT);
            SetShaderValue(_outline_shader, GetShaderLocation(_outline_shader, "time"), &_timer, SHADER_UNIFORM_FLOAT);
            DrawTextureRec(enemy_texture->get(), {0, 0, static_cast<float>(enemy_texture->get().width), static_cast<float>(enemy_texture->get().height)}, {static_cast<float>(x - (enemy_texture->get().width / 2)), static_cast<float>(y - enemy_texture->get().height)}, WHITE);
            EndShaderMode();
            EndBlendMode();
        }
    }
}


void EncounterView::_render_enemy_slot(auto idx, const auto &row_entities, const auto row_start_x, const auto row_y, const auto offset) {
    for (const auto &entity: row_entities) {
        if (entity != entt::null) {
            _render_enemy(entity, row_start_x + (idx * offset), row_y);
        }
        idx++;
    }
}

entt::entity EncounterView::_get_entity_at_point(auto idx, const auto &row_entities, auto row_start_x, auto row_y, auto offset, auto mouse_x, auto mouse_y) {
    for (auto entity: row_entities) {
        auto x = row_start_x + (idx * offset);
        auto y = row_y;
        if (auto *enemy_type = _core->registry.try_get<enemies::EnemyType>(entity)) {
            const auto *enemy_texture = &_core->get_assets()->_textures._enemies[enemy_type->value];
            Rectangle bounds = {static_cast<float>(x - (enemy_texture->get().width / 2)),
                                static_cast<float>(y - enemy_texture->get().height),
                                static_cast<float>(enemy_texture->get().width),
                                static_cast<float>(enemy_texture->get().height)};
            if (CheckCollisionPointRec(Vector2{static_cast<float>(mouse_x), static_cast<float>(mouse_y)}, bounds)) {
                return entity;
            }
        }
        idx++;
    }
    return entt::null;
}

void EncounterView::_clear_canvas() noexcept {
    UpdateTexture(_canvas.texture, _empty_canvas.data());
}
