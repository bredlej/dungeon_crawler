//
// Created by Patryk Szczypień on 11/08/2023.
//

#include <battle_view.hpp>


static void render_monster_selection_combo(const std::string &label, const std::vector<std::string> &monster_names, const std::vector<std::string> &monster_ids, std::string &selection, MonstersMap &monsters_map) {

    const auto preview_value = selection == "none" ? "None" : std::get<components::general::Name>(monsters_map.monsters[selection]).name;
    if (ImGui::BeginCombo(label.c_str(), preview_value.c_str())) {
        for (int cindex = 0; cindex < monster_ids.size(); cindex++) {
            const bool is_selected = (selection == monster_ids[cindex]);
            if (ImGui::Selectable(monster_names[cindex].c_str(), is_selected)) {
                selection = monster_ids[cindex];
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}
static void battle_setup(const std::shared_ptr<Core> &core, const std::string &name = "Battle setup") {
    if (ImGui::CollapsingHeader(name.c_str())) {
        /* Render 3x2 table with combo boxes to choose monsters from for the back and front row */

        if (!core->registry.ctx().contains<RowSetup>()) {
            core->registry.ctx().emplace<RowSetup>();
        }}
    if (const auto monsters_map = core->registry.ctx().find<MonstersMap>()) {
        std::vector<std::string> monster_names(monsters_map->monsters.size() + 1);
        std::vector<std::string> monster_ids(monster_names.size());

        auto i = 1;
        monster_names[0] = std::string("None");
        monster_ids[0] = "none";

        for (const auto& [fst, snd] : monsters_map->monsters) {
            monster_names[i]=  std::get<components::general::Name>(monsters_map->monsters[fst]).name;
            monster_ids[i] = fst;
            i++;
        }

        if (const auto row_setup = core->registry.ctx().find<RowSetup>()) {
            if(ImGui::BeginTable("positions", 3)) {
                ImGui::TableSetupColumn("I");
                ImGui::TableSetupColumn("II");
                ImGui::TableSetupColumn("III");
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                render_monster_selection_combo("##b0", monster_names, monster_ids, row_setup->back_row[0], *monsters_map);
                ImGui::TableNextColumn();
                render_monster_selection_combo("##b1", monster_names, monster_ids, row_setup->back_row[1], *monsters_map);
                ImGui::TableNextColumn();
                render_monster_selection_combo("##b2", monster_names, monster_ids, row_setup->back_row[2], *monsters_map);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                render_monster_selection_combo("##f0", monster_names, monster_ids, row_setup->front_row[0], *monsters_map);
                ImGui::TableNextColumn();
                render_monster_selection_combo("##f1", monster_names, monster_ids, row_setup->front_row[1], *monsters_map);
                ImGui::TableNextColumn();
                render_monster_selection_combo("##f2", monster_names, monster_ids, row_setup->front_row[2], *monsters_map);
                ImGui::EndTable();
            }
        }
    }
}

void BattleView::render() noexcept {
    using namespace battle;
    if (const auto *battle = _core->registry.ctx().find<Battle>()) {
        auto battle_phase = battle->battle_director->get_battle_phase();
        ImGui::Text("Battle phase: %s", to_string(battle_phase).c_str());
        ImGui::SameLine();
        if (ImGui::Button("End battle")) {
            _core->registry.ctx().erase<Battle>();
            _core->registry.ctx().erase<RowSetup>();
            battle_phase = BattlePhase::INACTIVE;
        }
        switch (battle_phase) {

            case BattlePhase::INACTIVE:
                battle_setup(_core);
                if (ImGui::Button("Start battle")) {
                    battle->battle_director->update();
                }
                break;
            case BattlePhase::BATTLE_START:
                if (!_core->registry.any_of<placement::TwoRows<3>>(battle->battle_entity)) {
                    if (const auto row_setup = _core->registry.ctx().find<RowSetup>()) {
                        //_core->registry.emplace<placement::TwoRows<3>>(battle->battle_entity, row_setup->front_row, row_setup->back_row);
                    }
                }
                if (ImGui::Button("Next phase")) {
                    battle->battle_director->update();
                }
                break;
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