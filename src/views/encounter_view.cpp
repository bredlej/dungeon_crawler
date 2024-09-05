//
// Created by Patryk Szczypień on 16/08/2024.
//

#include <views/encounter_view.hpp>
using namespace components::battle;

void EncounterView::_render_enemy_slot(auto idx, const auto &row_entities, const auto row_start_x, const auto row_y, const auto offset) {
    for (const auto &entity: row_entities) {
        if (entity != entt::null) {
            _render_enemy(entity, row_start_x + (idx * offset), row_y);
        }
        else {
            //DrawTextEx(_core->get_assets()->fonts.font, "{}", Vector2{static_cast<float>(row_start_x + (idx * offset)), static_cast<float>(row_y)}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
        }
        idx++;
    }
}

void EncounterView::render() noexcept {

    BeginDrawing();
    ClearBackground(BLACK);
    {
        BeginTextureMode(_canvas);
        {
            ClearBackground(BLACK);
            _battle_director->update();
            _render_enemies();
        }
        EndTextureMode();
    }
    static auto POV_DIMENSION_FULLSCREEN = Rectangle{0, 0, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    _render_texture(_canvas.texture, POV_DIMENSION_FULLSCREEN);
    _ui.render();
    EndDrawing();
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
        DrawTextureRec(enemy_texture->get(),  {0, 0, static_cast<float>(enemy_texture->get().width), static_cast<float>(enemy_texture->get().height)}, {static_cast<float>(x- (enemy_texture->get().width /2)), static_cast<float>(y - enemy_texture->get().height)}, WHITE);
        if (auto *name = _core->registry.try_get<components::general::Name>(entity)) {
            DrawTextEx(_core->get_assets()->fonts.font, name->name.c_str(), Vector2{static_cast<float>(x - (enemy_texture->get().width /2)), static_cast<float>(y - enemy_texture->get().height)}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
        }
    }
}

void EncounterView::update() noexcept {
    if (IsKeyReleased(KEY_DOWN)) {
        _core->dispatcher.enqueue(events::dungeon::EndEncounter{});
    }
}
