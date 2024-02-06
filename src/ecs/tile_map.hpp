//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_TILE_MAP_HPP
#define DUNGEON_CRAWLER_TILE_MAP_HPP

#include <cstdint>
#include <ecs/components.hpp>
#include <engine/core.hpp>
#include <engine/parsers/level_parser.hpp>
#include <entt/entt.hpp>
#include <ecs/components.hpp>
#include <random>
#include <utility>
#include <fmt/format.h>

struct Tile {
    entt::entity entity = entt::null;
};

struct NeighbourTile {
    entt::entity entity;
    WorldDirection direction;
};

constexpr auto kDEFAULT_MAP_SIZE = 20;

class TileMap {
public:
    explicit TileMap(std::shared_ptr<Core> core) : _core(core), _width{kDEFAULT_MAP_SIZE}, _height{kDEFAULT_MAP_SIZE}, _tiles{std::vector<Tile>(kDEFAULT_MAP_SIZE * kDEFAULT_MAP_SIZE)} {_initialize();};
    explicit TileMap(std::shared_ptr<Core> core, uint32_t width, uint32_t height) : _core(core), _width{width}, _height{height}, _tiles{std::vector<Tile>(width * height)} {_initialize();};
    TileMap(const TileMap &other) noexcept = delete;
    TileMap(TileMap &other) noexcept = delete;
    TileMap(TileMap &&other) noexcept = default;
    TileMap &operator=(const TileMap &other) noexcept = delete;
    std::vector<Tile> _tiles;
    uint32_t _width;
    uint32_t _height;
    [[nodiscard]] entt::entity get_at(int32_t x, int32_t y) const;
    [[nodiscard]] std::vector<NeighbourTile> get_neighbours_of (int32_t x, int32_t y) const;
    void from_json(nlohmann::json &);
    void to_json(nlohmann::json &);
private:
    void _initialize();

    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_TILE_MAP_HPP
