//
// Created by Patryk Szczypień on 16/08/2024.
//

#include <views/encounter_view.hpp>

void EncounterView::render() noexcept {
    BeginDrawing();
    ClearBackground(BLUE);
    _battle_director->update();
    EndDrawing();
}

void EncounterView::update() noexcept {
    if (IsKeyReleased(KEY_DOWN)) {
        _core->dispatcher.enqueue(events::dungeon::EndEncounter{});
    }
}

