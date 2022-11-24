//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_TILE_MAP_HPP
#define DUNGEON_CRAWLER_TILE_MAP_HPP

#include <entt/entt.hpp>
#include <cstdint>
#include <engine/core.hpp>
#include <utility>
#include <ecs/components.hpp>

struct Tile {
    entt::entity entity = entt::null;
};
constexpr auto kDEFAULT_MAP_SIZE = 20;

class TileMap {
public:
    explicit TileMap(std::shared_ptr<Core> core) : _core(std::move(core)), _width{kDEFAULT_MAP_SIZE}, _height{kDEFAULT_MAP_SIZE}, _tiles{std::vector<Tile>(kDEFAULT_MAP_SIZE * kDEFAULT_MAP_SIZE)} {_initialize();};
    explicit TileMap(std::shared_ptr<Core> core, uint32_t width, uint32_t height) : _core(std::move(core)), _width{width}, _height{height}, _tiles{std::vector<Tile>(width * height)} {};
    TileMap(const TileMap &other) noexcept = delete;
    TileMap(TileMap &other) noexcept = delete;
    TileMap(TileMap &&other) noexcept = default;
    TileMap &operator=(const TileMap &other) noexcept = delete;
    std::vector<Tile> _tiles;
    uint32_t _width;
    uint32_t _height;
private:
    void _initialize();

    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_TILE_MAP_HPP
