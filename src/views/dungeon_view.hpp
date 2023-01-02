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

template <typename T>
struct POVField {
    std::array<entt::entity, static_cast<size_t>(T::SIZE)> field;
};

class DungeonView : public UIView {
public:
    explicit DungeonView(std::shared_ptr<Core> &core, TileMap &&tile_map) : UIView{core}, _render_texture_pov{LoadRenderTexture(320, 240)}, _render_texture_gui(LoadRenderTexture(120, 120)), _wall_map{core}, _tile_map{std::move(tile_map)} {
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
    void _render_pov();
    void _render_minimap();
    void _calculate_fov();
    POVField<assets::dungeon_view::POVFloor> _player_fov_tile;
    POVField<assets::dungeon_view::POVWall> _player_fov_wall;
    RenderTexture _render_texture_pov;
    RenderTexture _render_texture_gui;
    TileMap _tile_map;
    WallMap _wall_map;
};


#endif//DUNGEON_CRAWLER_DUNGEON_VIEW_HPP
