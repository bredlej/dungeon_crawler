//
// Created by Patryk Szczypień on 07/04/2023.
//

#ifndef DUNGEON_CRAWLER_LEVEL_HPP
#define DUNGEON_CRAWLER_LEVEL_HPP
#include <core.hpp>
#include <level_parser.hpp>
#include <nlohmann/json.hpp>
#include <tile_map.hpp>
#include <wall_map.hpp>

class Level {
public:
    explicit Level(std::shared_ptr<Core> core, TileMap &&tile_map) : _core{core}, tile_map(std::move(tile_map)), wall_map(core) {
        wall_map.initialize(this->tile_map);
    }
    Level(const Level &) = delete;
    Level(Level &&) = default;
    Level &operator=(const Level &) = delete;
    Level &operator=(Level &&) = delete;
    ~Level() = default;

    void load(const std::string &path);
    void clear();
    void save(const std::string &path);
    TileMap tile_map;
    WallMap wall_map;
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_LEVEL_HPP
