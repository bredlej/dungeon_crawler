//
// Created by geoco on 19.11.2022.
//

#ifndef DUNGEON_CRAWLER_DUNGEON_VIEW_HPP
#define DUNGEON_CRAWLER_DUNGEON_VIEW_HPP

#include <Core.hpp>
#include <utility>
extern "C" {
#include <raylib.h>
};
#include <entt/entt.hpp>

struct Tile {
    entt::entity entity = entt::null;
};

class TileMap {
public:
    explicit TileMap() : _width{20}, _height{20}, _tiles{std::vector<Tile>(_width * _height)} {};
    explicit TileMap(uint32_t width, uint32_t height) : _width{width}, _height{height}, _tiles{std::vector<Tile>(width * height)} {};
    TileMap(const TileMap &other) noexcept = delete;
    TileMap(TileMap &other) noexcept = delete;
    TileMap(TileMap &&other) noexcept = default;
    TileMap &operator=(const TileMap &other) noexcept = delete;

private:
    std::vector<Tile> _tiles;
    uint32_t _width;
    uint32_t _height;
};

class DungeonView : public UIView {
public:
    explicit DungeonView(std::shared_ptr<Core> core, TileMap tile_map) : UIView{std::move(core)}, _tile_map{std::move(tile_map)} {};
    DungeonView(const DungeonView &) noexcept = delete;
    DungeonView(DungeonView &) noexcept = delete;
    DungeonView(DungeonView &&other) noexcept = delete;
    DungeonView &operator=(const DungeonView &) noexcept = delete;
    DungeonView &operator=(const DungeonView &&) noexcept = delete;

    void render() override;
    void update() override;

private:
    TileMap _tile_map;
};


#endif//DUNGEON_CRAWLER_DUNGEON_VIEW_HPP
