//
// Created by Patryk Szczypień on 10/04/2023.
//
#include <editor_assets.hpp>
void EditorAssets::load_textures() {

    _textures.emplace(FloorType::RUINS_01, std::string(game_path + assets::level_editor::tiles::ruins_01).c_str());
    _textures.emplace(FloorType::RUINS_02, std::string(game_path + assets::level_editor::tiles::ruins_02).c_str());
    _textures.emplace(MonsterType::GOBLIN, std::string(game_path + assets::monsters::goblin_warrior).c_str());
    _textures.emplace(MonsterType::GHOUL, std::string(game_path + assets::monsters::ghoul).c_str());
    _textures.emplace(MonsterType::SAND_SCORPION, std::string(game_path + assets::monsters::sand_scorpion).c_str());
    _textures.emplace(MonsterType::SPECTRE, std::string(game_path + assets::monsters::spectre).c_str());
    _textures.emplace(MonsterType::NOMAD_THIEF, std::string(game_path + assets::monsters::nomad_thief).c_str());
}