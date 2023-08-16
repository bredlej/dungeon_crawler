//
// Created by Patryk Szczypień on 11/08/2023.
//
#include <battle_view.hpp>

void BattleView::render() noexcept {
    const auto image = _core->registry.ctx().find<EditorAssets>()->_textures[MonsterType::SAND_SCORPION].get();
    DrawTextureRec(image,  {0, 0, static_cast<float>(image.width), static_cast<float>(image.height)}, {20, 20}, WHITE);
    using namespace editor;
}

void BattleView::update() noexcept {
    using namespace editor;
}