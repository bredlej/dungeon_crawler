//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>

void DungeonView::_initialize() noexcept{
    _core->registry.ctx().emplace<components::values::EncounterChance>(0.00f);
}
static inline void draw_floor(assets::Assets *assets, const size_t index, const FloorType floor_type, const Color tint) {
    DrawTexture(assets->_textures._tiles[static_cast<POVFloor>(index)][floor_type].get(), 0, 0, tint);
}

void DungeonView::_render_pov() noexcept {
    BeginTextureMode(_render_texture_pov);
    ClearBackground(BACKGROUND_COLOR);
    if (assets::Assets *assets = _core->get_assets()) {
        for (size_t i = 0; i < _player_fov_tile.field.size(); i++) {
            if (_core->registry.valid(_player_fov_tile.field[i])) {
                const auto floor = _core->registry.try_get<components::fields::Floor>(_player_fov_tile.field[i]);
                if (floor) {
                    draw_floor(assets, i, floor->type, WHITE);
                }
                else {
                    draw_floor(assets, i, FloorType::RUINS_01, RED);
                }
            } else {
                draw_floor(assets, i, FloorType::RUINS_01, RED);
            }
        }
        for (const POVWall i: draw_order_walls) {
            if (_core->registry.valid(_player_fov_wall.field[static_cast<const size_t>(i)])) {
                if (const components::fields::Wall *wall = _core->registry.try_get<components::fields::Wall>(_player_fov_wall.field[static_cast<const size_t>(i)])) {
                    DrawTexture(assets->_textures._tiles[static_cast<POVWall>(i)][wall->type].get(), 0, 0, WHITE);
                }
            }
        }
        if (_core->registry.ctx().contains<components::values::Encounter>()) {
            DrawTexture(assets->_textures._beasts[Beast::GoblinWarrior].get(), 140, 55, WHITE);
        }
    }
    EndTextureMode();
}

static inline void minimap_draw_player_frame(const Texture2D &texture, const Rectangle &frame, components::fields::MapPosition &position, ModXY offset) {
    DrawTextureRec(texture, frame, Vector2{static_cast<float>(position.x * MINIMAP_GRID_SIZE + offset.x), static_cast<float>(position.y * MINIMAP_GRID_SIZE + offset.y)}, WHITE);
}

void DungeonView::_render_minimap() noexcept {

    BeginTextureMode(_render_texture_gui);
    ClearBackground(BACKGROUND_COLOR);
    DrawTexture(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Background].get(), 0, 0, WHITE);
    ModXY offset {10,10};
    for (auto tile: _level.tile_map._tiles) {
        if (_core->registry.valid(tile.entity)) {
            components::fields::MapPosition position = _core->registry.get<components::fields::MapPosition>(tile.entity);
            auto *tile_in_fov = _core->registry.try_get<components::fields::InFovOfEntity>(tile.entity);
            if (tile_in_fov) {
                DrawRectangle(position.x * MINIMAP_GRID_SIZE + offset.x, position.y * MINIMAP_GRID_SIZE +offset.y, MINIMAP_GRID_SIZE, MINIMAP_GRID_SIZE, FOV_COLOR);
            }
            else {
                DrawRectangle(position.x * MINIMAP_GRID_SIZE + offset.x, position.y * MINIMAP_GRID_SIZE +offset.y, MINIMAP_GRID_SIZE, MINIMAP_GRID_SIZE, BACKGROUND_COLOR);
            }
        }
    }
    auto player_view = _core->registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    Texture2D player_texture = _core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get();
    player_view.each([&](const entt::entity entity, const components::general::Player player, components::general::Direction direction, components::fields::MapPosition position) {
        switch (direction.direction) {
            case WorldDirection::NORTH: minimap_draw_player_frame(player_texture, {0,0,5,5}, position, offset); break;
            case WorldDirection::EAST: minimap_draw_player_frame(player_texture, {5,0,5,5}, position, offset); break;
            case WorldDirection::SOUTH: minimap_draw_player_frame(player_texture, {10,0,5,5}, position, offset); break;
            case WorldDirection::WEST: minimap_draw_player_frame(player_texture, {15,0,5,5}, position, offset); break;
        }
    });
    for (WallEntity wall: _level.wall_map._walls) {
        components::fields::Wall wall_component = _core->registry.get<components::fields::Wall>(wall.entity);
        components::fields::MapPosition field1_position = wall_component.field1;
        components::fields::MapPosition field2_position = wall_component.field2;
        if (field1_position.x == field2_position.x) {
            DrawLine(field1_position.x * MINIMAP_GRID_SIZE + offset.x, std::max(field1_position.y, field2_position.y) * MINIMAP_GRID_SIZE + offset.y, field1_position.x * MINIMAP_GRID_SIZE + offset.x + MINIMAP_GRID_SIZE, std::max(field1_position.y, field2_position.y) * 5 + offset.y, WALL_COLOR);
        }
        else if (field1_position.y == field2_position.y) {
            DrawLine(std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y, std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y + MINIMAP_GRID_SIZE, WALL_COLOR);
        }
    }
    EndTextureMode();
}

static inline void render_texture(const Texture &texture, const Rectangle &dimension) {
    DrawTexturePro(texture,
                   Rectangle{0.0f, 0.0f, (float) texture.width, (float) -texture.height},
                   dimension,
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
}

void DungeonView::render() noexcept {
    BeginDrawing();
    ClearBackground(BLACK);
    _render_pov();
    _render_minimap();
    static Rectangle POV_DIMENSION_FULLSCREEN = Rectangle{0,0, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    static Rectangle GUI_DIMENSION_FULLSCREEN = Rectangle{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.75f, static_cast<float>(GetMonitorHeight(GetCurrentMonitor())) * 0.1f, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.2f, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.2f};
    render_texture(_render_texture_pov.texture, IsWindowFullscreen() ? POV_DIMENSION_FULLSCREEN : POV_DIMENSION);
    render_texture(_render_texture_gui.texture, IsWindowFullscreen() ? GUI_DIMENSION_FULLSCREEN : GUI_DIMENSION);
    _ui.render();
    EndDrawing();
}

void DungeonView::update() noexcept {
    static bool after_first_update = false;
    static bool recalculate_fov = true;
    if (recalculate_fov) {
        _core->registry.ctx().emplace<events::dungeon::RecalculateFov>();
        recalculate_fov = false;
    }
    if (after_first_update && _core->registry.ctx().contains<events::dungeon::RecalculateFov>()) {
        _calculate_fov();
        _core->registry.ctx().erase<events::dungeon::RecalculateFov>();
    }
    if (IsKeyPressed(KEY_LEFT)) {
        _core->dispatcher.enqueue(events::dungeon::TurnLeft{});
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        _core->dispatcher.enqueue(events::dungeon::TurnRight{});
    }
    if (IsKeyPressed(KEY_UP)) {
        _core->dispatcher.enqueue(events::dungeon::MoveForward{});
    }
    if (IsKeyPressed(KEY_DOWN)) {
        _core->dispatcher.enqueue(events::dungeon::MoveBack{});
    }
    if (IsKeyPressed(KEY_L)) {
        try {
            _clear();
            _level.load("assets/Levels/Ruins/ruins_01.json");
            _calculate_fov();
        }
        catch (std::exception &e) {
            printf("Exception: %s\n", e.what());
        }
    }
    if (IsKeyPressed(KEY_S)) {
        try {
            _level.save("assets/Levels/Ruins/ruins_01_saved.json");
        }
        catch (std::exception &e) {
            printf("Exception: %s\n", e.what());
        }
    }
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_D)) {
        _core->dispatcher.enqueue(events::ui::ToggleShowDemo());
    }
    after_first_update = true;
}

template<size_t AMOUNT_WALLS_IN_FOV>
static void fill_player_fov_walls(std::array<entt::entity, AMOUNT_WALLS_IN_FOV> &player_fov_walls, const components::fields::MapPosition player_position, const WallMap &wall_map, const TileMap &tile_map, const WorldDirection direction) {
    using namespace components::fields;
    ModXY mod = (direction == WorldDirection::NORTH || direction == WorldDirection::EAST) ? ModXY{1, 1} : ModXY{-1, -1};
    // 01 - 05
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 2 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x - 2 * mod.x, player_position.y - 5 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x + 5 * mod.x, player_position.y - 2 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 2 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x - 2 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y - 2 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 2 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y - 4 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x + 4 * mod.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y - 5 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 5 * mod.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x, player_position.y - 4 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 4 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x, player_position.y - 5 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y}, MapPosition{player_position.x + 5 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 5 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y + 1 * mod.y}, MapPosition{player_position.x + 5 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y + 1 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 4 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y}, MapPosition{player_position.x + 4 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y - 5 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y + 2 * mod.y}, MapPosition{player_position.x + 5 * mod.x, player_position.y + 2 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y + 2 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y + 2 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y - 4 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 4 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 4 * mod.x, player_position.y + 2 * mod.y}, MapPosition{player_position.x + 4 * mod.x, player_position.y + 1 * mod.y});
    // 06 - 10                                                                                                                                                                   MapPosition{
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W06_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 2 * mod.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y - 2 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W08_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x, player_position.y - 2 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 2 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y + 1 * mod.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W10_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y - 3 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 3 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 3 * mod.x, player_position.y + 1 * mod.y}, MapPosition{player_position.x + 3 * mod.x, player_position.y + 2 * mod.y});
    // 11 - 13                                                                                                                                                                   MapPosition{
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y - 1 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x, player_position.y - 2 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W12_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x, player_position.y - 1 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 2 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y}, MapPosition{player_position.x + 2 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 2 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 1 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 2 * mod.x, player_position.y + 1 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y + 1 * mod.y});
    // 14 - 16                                                                                                                                                                   MapPosition{
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x - 1 * mod.x, player_position.y}) : wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x, player_position.y - 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x, player_position.y - 1 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W15_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x, player_position.y}) : wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y}, MapPosition{player_position.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y - 1 * mod.y}) : wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y + 1 * mod.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y}) : wall_map.get_between(MapPosition{player_position.x + 1 * mod.x, player_position.y + 1 * mod.y}, MapPosition{player_position.x, player_position.y + 1 * mod.y});
    // 17 - 19                                                                                                                                                                   MapPosition{
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W17_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x - 1 * mod.x, player_position.y}, MapPosition{player_position.x, player_position.y}) : wall_map.get_between(MapPosition{player_position.x, player_position.y - 1 * mod.y}, MapPosition{player_position.x, player_position.y});
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W19_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(MapPosition{player_position.x, player_position.y}, MapPosition{player_position.x + 1 * mod.x, player_position.y}) : wall_map.get_between(MapPosition{player_position.x, player_position.y}, MapPosition{player_position.x, player_position.y + 1 * mod.y});
}

template <size_t SIZE>
static void fill_player_fov_tiles(std::array<entt::entity, SIZE> &player_fov_tiles, const components::fields::MapPosition player_position, const TileMap &tile_map, const WorldDirection direction) {
    ModXY mod = (direction == WorldDirection::NORTH || direction == WorldDirection::EAST) ? ModXY{1, 1} : ModXY{-1, -1};
    
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F01] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 4 * mod.y) : tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 2 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F02] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 4 * mod.y) : tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F03] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 4 * mod.y) : tile_map.get_at(player_position.x + 4 * mod.x, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F04] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 4 * mod.y) : tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F05] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 4 * mod.y) : tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 2 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F06] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 3 * mod.y) : tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 2 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F07] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 3 * mod.y) : tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F08] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 3 * mod.y) : tile_map.get_at(player_position.x + 3 * mod.x, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F09] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 3 * mod.y) : tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F10] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 3 * mod.y) : tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 2 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F11] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 2 * mod.y) : tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F12] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 2 * mod.y) : tile_map.get_at(player_position.x + 2 * mod.x, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F13] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 2 * mod.y) : tile_map.get_at(player_position.x + 2 * mod.x, player_position.y + 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F14] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 1 * mod.y) : tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F15] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 1 * mod.y) : tile_map.get_at(player_position.x + 1 * mod.x, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F16] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y) : tile_map.get_at(player_position.x + 1 * mod.x, player_position.y + 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F17] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * mod.x, player_position.y) : tile_map.get_at(player_position.x, player_position.y - 1 * mod.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F18] = tile_map.get_at(player_position.x, player_position.y); // the player is always on field 18
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F19] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * mod.x, player_position.y) : tile_map.get_at(player_position.x, player_position.y + 1 * mod.y);
}

void DungeonView::_calculate_fov() noexcept {
    _core->registry.clear<components::fields::InFovOfEntity>();
    auto player_view = _core->registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    for (auto entity: player_view) {
        auto player_position = player_view.get<components::fields::MapPosition>(entity);
        auto player_direction = player_view.get<components::general::Direction>(entity);

        fill_player_fov_tiles(_player_fov_tile.field, player_position, _level.tile_map, player_direction.direction);
        fill_player_fov_walls(_player_fov_wall.field, player_position, _level.wall_map, _level.tile_map, player_direction.direction);

        for (entt::entity fov_tile: _player_fov_tile.field) {
            if (_core->registry.valid(fov_tile)) {
                _core->registry.emplace_or_replace<components::fields::InFovOfEntity>(fov_tile, entity);
            }
        }
    }
}

void DungeonView::_clear() noexcept {
    _level.clear();
    _player_fov_tile.field.fill(entt::null);
    _player_fov_wall.field.fill(entt::null);
}
