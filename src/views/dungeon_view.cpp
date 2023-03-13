//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>

void DungeonView::_render_pov() {
    BeginTextureMode(_render_texture_pov);
    ClearBackground(BACKGROUND_COLOR);
    if (assets::Assets *assets = _core->get_assets()) {
        for (size_t i = 0; i < _player_fov_tile.field.size(); i++) {
            if (_core->_registry.valid(_player_fov_tile.field[i])) {
                const auto floor = _core->_registry.try_get<components::fields::Floor>(_player_fov_tile.field[i]);
                if (floor) {
                    DrawTexture(assets->_textures._tiles[static_cast<POVFloor>(i)][floor->type].get(), 0, 0, WHITE);
                }
                else {
                    DrawTexture(assets->_textures._tiles[static_cast<POVFloor>(i)][FloorType::RUINS_01].get(), 0, 0, RED);
                }
            } else {
                DrawTexture(assets->_textures._tiles[static_cast<POVFloor>(i)][FloorType::RUINS_01].get(), 0, 0, RED);
            }
        }
        for (const POVWall i: draw_order_walls) {
            if (_core->_registry.valid(_player_fov_wall.field[static_cast<const size_t>(i)])) {
                if (const components::fields::Wall *wall = _core->_registry.try_get<components::fields::Wall>(_player_fov_wall.field[static_cast<const size_t>(i)])) {
                    DrawTexture(assets->_textures._tiles[static_cast<POVWall>(i)][wall->type].get(), 0, 0, WHITE);
                }
            }
        }
    }
    EndTextureMode();
}

static inline void minimap_draw_player_frame(const Texture2D &texture, const Rectangle &frame, components::fields::MapPosition &position, ModXY offset) {
    DrawTextureRec(texture, frame, Vector2{static_cast<float>(position.x * MINIMAP_GRID_SIZE + offset.x), static_cast<float>(position.y * MINIMAP_GRID_SIZE + offset.y)}, WHITE);
}
void DungeonView::_render_minimap() {
    BeginTextureMode(_render_texture_gui);
    ClearBackground(BACKGROUND_COLOR);
    DrawTexture(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Background].get(), 0, 0, WHITE);
    ModXY offset {10,10};
    for (auto tile: _tile_map._tiles) {
        if (_core->_registry.valid(tile.entity)) {
            components::fields::MapPosition position = _core->_registry.get<components::fields::MapPosition>(tile.entity);
            auto *tile_in_fov = _core->_registry.try_get<components::fields::InFovOfEntity>(tile.entity);
            if (tile_in_fov) {
                DrawRectangle(position.x * MINIMAP_GRID_SIZE + offset.x, position.y * MINIMAP_GRID_SIZE +offset.y, MINIMAP_GRID_SIZE, MINIMAP_GRID_SIZE, FOV_COLOR);
            }
            else {
                DrawRectangle(position.x * MINIMAP_GRID_SIZE + offset.x, position.y * MINIMAP_GRID_SIZE +offset.y, MINIMAP_GRID_SIZE, MINIMAP_GRID_SIZE, BACKGROUND_COLOR);
            }
        }
    }
    auto player_view = _core->_registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    Texture2D player_texture = _core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get();
    player_view.each([&](const entt::entity entity, const components::general::Player player, components::general::Direction direction, components::fields::MapPosition position) {
        switch (direction.direction) {
            case WorldDirection::NORTH: minimap_draw_player_frame(player_texture, {0,0,5,5}, position, offset); break;
            case WorldDirection::EAST: minimap_draw_player_frame(player_texture, {5,0,5,5}, position, offset); break;
            case WorldDirection::SOUTH: minimap_draw_player_frame(player_texture, {10,0,5,5}, position, offset); break;
            case WorldDirection::WEST: minimap_draw_player_frame(player_texture, {15,0,5,5}, position, offset); break;
        }
    });
    for (WallEntity wall: _wall_map._walls) {
        components::fields::Wall wall_component = _core->_registry.get<components::fields::Wall>(wall.entity);
        components::fields::MapPosition field1_position = _core->_registry.get<components::fields::MapPosition>(wall_component.field1);
        components::fields::MapPosition field2_position = _core->_registry.get<components::fields::MapPosition>(wall_component.field2);
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

void DungeonView::render() {
    BeginDrawing();
    ClearBackground(BLACK);
    _render_pov();
    _render_minimap();
    render_texture(_render_texture_pov.texture, POV_DIMENSION);
    render_texture(_render_texture_gui.texture, GUI_DIMENSION);
    EndDrawing();
}

static inline void change_position(ModXY mod, entt::registry &registry, const TileMap &tile_map, const WallMap &wall_map, components::fields::MapPosition &position) {
    entt::entity destination = tile_map.get_at(position.x + mod.x, position.y + mod.y);
    if (registry.valid(destination) && registry.get<components::fields::Walkability>(destination).is_walkable) {
        entt::entity wall = wall_map.get_between(tile_map.get_at(position.x, position.y), destination);
        if (wall == entt::null || registry.get<components::fields::Walkability>(wall).is_walkable) {
            components::fields::MapPosition destination_position = registry.get<components::fields::MapPosition>(destination);
            position.x = destination_position.x;
            position.y = destination_position.y;
        }
    }
}

static inline void handle_movement (entt::registry &registry, TileMap &tile_map, const WallMap &wall_map, ModXY mod_north, ModXY mod_south, ModXY mod_west, ModXY mod_east) {
    auto view = registry.view<components::general::Player, components::fields::MapPosition, components::general::Direction>();
    for (auto entity: view) {
        auto &position = registry.get<components::fields::MapPosition>(entity);
        auto direction = registry.get<components::general::Direction>(entity);
        switch (direction.direction) {
            case WorldDirection::NORTH: change_position(mod_north, registry, tile_map, wall_map, position); break;
            case WorldDirection::SOUTH: change_position(mod_south, registry, tile_map, wall_map, position); break;
            case WorldDirection::WEST: change_position(mod_west, registry, tile_map, wall_map, position); break;
            case WorldDirection::EAST: change_position(mod_east, registry, tile_map, wall_map, position); break;
        }
    }
}

static inline void handle_turn_direction (entt::registry &registry, WorldDirection from_north, WorldDirection from_east, WorldDirection from_south, WorldDirection from_west) {
    registry.view<components::general::Player, components::general::Direction>().each([from_north, from_east, from_south, from_west](const entt::entity entity, const components::general::Player player, components::general::Direction &direction) {
        switch (direction.direction) {
            case WorldDirection::NORTH: direction.direction = from_north; break;
            case WorldDirection::EAST: direction.direction = from_east; break;
            case WorldDirection::SOUTH: direction.direction = from_south; break;
            case WorldDirection::WEST: direction.direction = from_west; break;
        }
    });
}

void DungeonView::update() {
    static bool after_first_update = false;
    static bool recalculate_fov = true;
    if (after_first_update && recalculate_fov) {
        _calculate_fov();
        recalculate_fov = false;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        handle_turn_direction(_core->_registry, WorldDirection::WEST, WorldDirection::NORTH, WorldDirection::EAST, WorldDirection::SOUTH);
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        handle_turn_direction(_core->_registry, WorldDirection::EAST, WorldDirection::SOUTH, WorldDirection::WEST, WorldDirection::NORTH);
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_UP)) {
        handle_movement(_core->_registry, _tile_map, _wall_map, {0, -1}, {0, 1}, {-1, 0}, {1, 0});
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        handle_movement(_core->_registry, _tile_map, _wall_map, {0, 1}, {0, -1}, {1, 0}, {-1, 0});
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_L)) {
        try {
            auto json = LevelParser::parse("assets/Levels/Ruins/ruins_01.json");
            _clear();
            _tile_map.from_json(json);
            _wall_map.from_json(_tile_map, json);
            using namespace level_schema;
            if (json.contains(names[types::player_spawn])) {
                _core->_registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>().each([&json](const entt::entity entity, const components::general::Player player, components::general::Direction &direction, components::fields::MapPosition &position) {
                   direction.direction = direction_names[json[names[types::player_spawn]][names[types::direction]]];
                   position.x = json[names[types::player_spawn]][names[types::x]];
                   position.y = json[names[types::player_spawn]][names[types::y]];
                });

            }
            _calculate_fov();
        }
        catch (std::exception &e) {
            printf("Exception: %s\n", e.what());
        }

    }
    after_first_update = true;
}

template<size_t AMOUNT_WALLS_IN_FOV>
static void fill_player_fov_walls(std::array<entt::entity, AMOUNT_WALLS_IN_FOV> &player_fov_walls, const components::fields::MapPosition player_position, const WallMap &wall_map, const TileMap &tile_map, const WorldDirection direction) {
    ModXY mod = (direction == WorldDirection::NORTH || direction == WorldDirection::EAST) ? ModXY{1, 1} : ModXY{-1, -1};
    // 01 - 05
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 5 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x + 5 * mod.x, player_position.y - 2 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 2 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 4 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 5 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 5 * mod.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x, player_position.y - 4 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 4 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x, player_position.y - 5 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y), tile_map.get_at(player_position.x + 5 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 5 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 1 * mod.y), tile_map.get_at(player_position.x + 5 * mod.x, player_position.y  + 1* mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 1 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y  + 1* mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 4 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y), tile_map.get_at(player_position.x + 4 * mod.x, player_position.y +1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 5 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 2 * mod.y), tile_map.get_at(player_position.x + 5 * mod.x, player_position.y + 2 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 2 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 2 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 4 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 4 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 2 * mod.y), tile_map.get_at(player_position.x + 4 * mod.x, player_position.y + 1 * mod.y));
    // 06 - 10
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W06_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 2 * mod.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 2 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W08_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x, player_position.y - 2 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 2 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 1 * mod.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y + 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W10_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 3 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 3 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 1 * mod.y), tile_map.get_at(player_position.x + 3 * mod.x, player_position.y + 2 * mod.y));
    // 11 - 13
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 1 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x, player_position.y - 2 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W12_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x, player_position.y - 1 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 2 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y), tile_map.get_at(player_position.x + 2 * mod.x, player_position.y + 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 2 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 2 * mod.x, player_position.y + 1 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y + 1* mod.y));
    // 14 - 16
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x - 1 * mod.x, player_position.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x, player_position.y - 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x, player_position.y - 1 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W15_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x, player_position.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y), tile_map.get_at(player_position.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y + 1 * mod.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y)) : wall_map.get_between(tile_map.get_at(player_position.x + 1 * mod.x, player_position.y + 1 * mod.y), tile_map.get_at(player_position.x, player_position.y + 1 * mod.y));
    // 17 - 19
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W17_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x - 1 * mod.x, player_position.y), tile_map.get_at(player_position.x, player_position.y)) : wall_map.get_between(tile_map.get_at(player_position.x, player_position.y - 1 * mod.y), tile_map.get_at(player_position.x, player_position.y));
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W19_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? wall_map.get_between(tile_map.get_at(player_position.x, player_position.y), tile_map.get_at(player_position.x + 1 * mod.x, player_position.y)) : wall_map.get_between(tile_map.get_at(player_position.x, player_position.y), tile_map.get_at(player_position.x, player_position.y + 1 * mod.y));
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

void DungeonView::_calculate_fov() {
    _core->_registry.clear<components::fields::InFovOfEntity>();
    auto player_view = _core->_registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    for (auto entity: player_view) {
        auto player_position = player_view.get<components::fields::MapPosition>(entity);
        auto player_direction = player_view.get<components::general::Direction>(entity);

        fill_player_fov_tiles(_player_fov_tile.field, player_position, _tile_map, player_direction.direction);
        fill_player_fov_walls(_player_fov_wall.field, player_position, _wall_map, _tile_map, player_direction.direction);

        for (entt::entity fov_tile: _player_fov_tile.field) {
            if (_core->_registry.valid(fov_tile)) {
                _core->_registry.emplace_or_replace<components::fields::InFovOfEntity>(fov_tile, entity);
            }
        }
    }
}
void DungeonView::_clear() {
    for (auto wall_entity: _wall_map._walls) {
        _core->_registry.destroy(wall_entity.entity);
    }
    _wall_map._walls.clear();
    for (auto tile_entity: _tile_map._tiles) {
        _core->_registry.destroy(tile_entity.entity);
    }
    _tile_map._tiles.clear();
    _player_fov_tile.field.fill(entt::null);
    _player_fov_wall.field.fill(entt::null);
}
