//
// Created by Patryk Szczypień on 11/08/2023.
//

#include <battle_view.hpp>

void BattleView::render() noexcept {
    using namespace battle;
    if (const auto *battle = _core->registry.ctx().find<Battle>()) {
        const auto battle_phase = battle->battle_director->get_battle_phase();
        ImGui::Text("Battle phase: %s", to_string(battle_phase).c_str());
        switch (battle_phase) {

            case BattlePhase::INACTIVE:
                if (ImGui::Button("Start battle")) {
                    battle->battle_director->update();
                }
                break;
            case BattlePhase::BATTLE_START:
            case BattlePhase::TURN_START:
            case BattlePhase::PLAYER_ACTIONS:
            case BattlePhase::AI_ACTIONS:
            case BattlePhase::TURN_END:
            case BattlePhase::BATTLE_END:
            {
                const auto spectre = _core->registry.ctx().find<EditorAssets>()->_textures[MonsterType::SPECTRE].get();
                const auto nomad_thief = _core->registry.ctx().find<EditorAssets>()->_textures[MonsterType::NOMAD_THIEF].get();
                const auto ghoul = _core->registry.ctx().find<EditorAssets>()->_textures[MonsterType::GHOUL].get();
                DrawTextureRec(spectre,  {0, 0, static_cast<float>(spectre.width), static_cast<float>(spectre.height)}, {20, 40}, WHITE);
                DrawTextureRec(nomad_thief,  {0, 0, static_cast<float>(nomad_thief.width), static_cast<float>(nomad_thief.height)}, {120, 40}, WHITE);
                DrawTextureRec(ghoul,  {0, 0, static_cast<float>(ghoul.width), static_cast<float>(ghoul.height)}, {260, 80}, WHITE);
                if (ImGui::Button("Next phase")) {
                    battle->battle_director->update();
                }
                break;
            }
            case BattlePhase::FINISHED:
                if (ImGui::Button("End battle")) {
                    _core->registry.ctx().erase<Battle>();
                }
                break;
        }
    }
    else {
        if (ImGui::Button("New battle")) {
            _core->registry.ctx().emplace<Battle>(_battle_system.start_battle());
            _core->registry.ctx().find<Battle>()->battle_director->end_condition = [](const std::shared_ptr<Core> &core) {
                return false;
            };

        }
    }

}

void BattleView::update() noexcept {
    using namespace editor;
}