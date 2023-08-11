//
// Created by Patryk Szczypień on 11/08/2023.
//
#include <battle_view.hpp>

void BattleView::render() noexcept {
    DrawTextureRec(_core->registry.ctx().find<EditorAssets>()->_textures[MonsterType::SAND_SCORPION].get(), {0, 0, 25, 25}, {0, 0}, WHITE);
    using namespace editor;
}

void BattleView::update() noexcept {
    using namespace editor;
}