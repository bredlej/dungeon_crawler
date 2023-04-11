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
#include <views/ui/dungeon_ui.hpp>
#include <views/actions/dungeon_actions.hpp>
#include <events.hpp>
#include <level.hpp>

constexpr static Color BACKGROUND_COLOR = {0x34, 0x31, 0x1d, 0xff};
constexpr static Color FOV_COLOR = {0x48, 0x53, 0x22, 0xff};
constexpr static Color WALL_COLOR = {0xb1, 0x48, 0x63, 0xff};
constexpr static Rectangle POV_DIMENSION = Rectangle{0.0f, 0.0f, static_cast<float>(1024.0f), static_cast<float>(768.0f)};
constexpr static Rectangle GUI_DIMENSION = Rectangle{750.0f, 50.0f, static_cast<float>(240.0f), static_cast<float>(240.0f)};
constexpr static uint8_t MINIMAP_GRID_SIZE = 5;
//clang-format off
using namespace assets::dungeon_view;
constexpr static std::array<POVWall, static_cast<size_t>(POVWall::SIZE)> draw_order_walls = {
        POVWall::W01_N, POVWall::W02_N, POVWall::W03_N, POVWall::W04_N, POVWall::W05_N, POVWall::W01_E, POVWall::W02_E, POVWall::W04_W, POVWall::W05_W, POVWall::W01_S, POVWall::W02_S, POVWall::W03_S, POVWall::W04_S, POVWall::W05_S,
        POVWall::W06_E, POVWall::W07_E, POVWall::W09_W, POVWall::W10_W, POVWall::W07_S, POVWall::W08_S, POVWall::W09_S,
        POVWall::W11_E, POVWall::W13_W, POVWall::W11_S, POVWall::W12_S, POVWall::W13_S,
        POVWall::W14_E, POVWall::W16_W, POVWall::W14_S, POVWall::W15_S, POVWall::W16_S,
        POVWall::W17_E, POVWall::W19_W
};
//clang-format on
template <typename T, typename = typename std::enable_if<std::is_enum<T>::value, T >::type>
struct POVField {
    std::array<entt::entity, static_cast<size_t>(T::SIZE)> field;
};

class DungeonView : public UIView<DungeonView> {
public:
    explicit DungeonView(std::shared_ptr<Core> &core, Level &&level) : UIView{core}, _ui{core}, _render_texture_pov{LoadRenderTexture(320, 240)}, _render_texture_gui(LoadRenderTexture(120, 120)), _level{std::move(level)} {
        _initialize();
        std::printf("Dungeon View constructed\n");
    };
    DungeonView(const DungeonView &) noexcept = delete;
    DungeonView(DungeonView &) noexcept = delete;
    DungeonView(DungeonView &&other) noexcept = delete;
    DungeonView &operator=(const DungeonView &) noexcept = delete;
    DungeonView &operator=(const DungeonView &&) noexcept = delete;

    void render() noexcept override;
    void update() noexcept override;
private:
    void _initialize() noexcept;
    void _render_pov() noexcept;
    void _render_minimap() noexcept;
    void _calculate_fov() noexcept;
    void _clear() noexcept;
    POVField<assets::dungeon_view::POVFloor> _player_fov_tile;
    POVField<assets::dungeon_view::POVWall> _player_fov_wall;
    RenderTexture _render_texture_pov;
    RenderTexture _render_texture_gui;
    Level _level;
    DungeonUI _ui;
    DungeonActions _actions{_core, &_level};
};


#endif//DUNGEON_CRAWLER_DUNGEON_VIEW_HPP
