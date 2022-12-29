//
// Created by geoco on 19.11.2022.
//

#ifndef DUNGEON_CRAWLER_DUNGEON_VIEW_HPP
#define DUNGEON_CRAWLER_DUNGEON_VIEW_HPP

#include "ecs/wall_map.hpp"
#include "engine/core.hpp"
#include <utility>
extern "C" {
#include <raylib.h>
};
#include <ecs/tile_map.hpp>

class DungeonView : public UIView {
public:
    explicit DungeonView(std::shared_ptr<Core> &core, TileMap &&tile_map) : UIView{core}, _render_texture{LoadRenderTexture(320, 240)}, _wall_map{core}, _tile_map{std::move(tile_map)} {
        _wall_map.initialize(_tile_map);
        std::printf("Dungeon View constructed\n");
    };
    DungeonView(const DungeonView &) noexcept = delete;
    DungeonView(DungeonView &) noexcept = delete;
    DungeonView(DungeonView &&other) noexcept = delete;
    DungeonView &operator=(const DungeonView &) noexcept = delete;
    DungeonView &operator=(const DungeonView &&) noexcept = delete;

    void render() override;
    void update() override;
private:
    void _calculate_fov();
    std::vector<entt::entity> _player_fov_tile{19};
    std::vector<entt::entity> _player_fov_wall{28};
    RenderTexture _render_texture;
    TileMap _tile_map;
    WallMap _wall_map;
};


#endif//DUNGEON_CRAWLER_DUNGEON_VIEW_HPP
