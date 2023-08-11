//
// Created by Patryk Szczypień on 07/04/2023.
//

#ifndef DUNGEON_CRAWLER_LEVEL_LIST_HPP
#define DUNGEON_CRAWLER_LEVEL_LIST_HPP
#include <filesystem>
#include <string>
#include <vector>
extern "C" {
#include <raylib.h>
};
#include <events.hpp>
#include <core.hpp>
#include <cstdlib>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>

class LevelList {
public:
    explicit LevelList(std::shared_ptr<Core> core) : _core{core} {};
    LevelList(const LevelList &) = delete;
    LevelList(LevelList &&) = delete;
    LevelList &operator=(const LevelList &) = delete;
    LevelList &operator=(LevelList &&) = delete;
    ~LevelList() = default;
    void render();
    void update();
    void find_levels(const std::string &path);

private:
    bool _visible{true};
    std::shared_ptr<Core> _core;
    std::vector<std::filesystem::path> _levels;
};
#endif//DUNGEON_CRAWLER_LEVEL_LIST_HPP
