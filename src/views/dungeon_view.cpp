//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>
using namespace assets::dungeon_view;

constexpr static Color BACKGROUND_COLOR = {0x34, 0x31, 0x1d, 0xff};
constexpr static Color FOV_COLOR = {0x48, 0x53, 0x22, 0xff};
constexpr static Color WALL_COLOR = {0xb1, 0x48, 0x63, 0xff};
constexpr static Rectangle POV_DIMENSION = Rectangle{0.0f, 0.0f, static_cast<float>(1024.0f), static_cast<float>(768.0f)};
constexpr static Rectangle GUI_DIMENSION = Rectangle{700.0f, 50.0f, static_cast<float>(240.0f), static_cast<float>(240.0f)};

constexpr static std::array<assets::dungeon_view::POVWall, static_cast<size_t>(POVWall::SIZE)> draw_order_walls = {
    POVWall::W01_N, POVWall::W02_N, POVWall::W03_N, POVWall::W04_N, POVWall::W05_N, POVWall::W01_E, POVWall::W02_E, POVWall::W04_W, POVWall::W05_W, POVWall::W01_S, POVWall::W02_S, POVWall::W03_S, POVWall::W04_S, POVWall::W05_S,
    POVWall::W06_E, POVWall::W07_E, POVWall::W09_W, POVWall::W10_W, POVWall::W07_S, POVWall::W08_S, POVWall::W09_S,
    POVWall::W11_E, POVWall::W13_W, POVWall::W11_S, POVWall::W12_S, POVWall::W13_S,
    POVWall::W14_E, POVWall::W16_W, POVWall::W14_S, POVWall::W15_S, POVWall::W16_S,
    POVWall::W17_E, POVWall::W19_W
};
//clang-format on

void DungeonView::_render_pov() {
    BeginTextureMode(_render_texture_pov);
    ClearBackground(BACKGROUND_COLOR);
    if (assets::Assets *assets = _core->get_assets()) {
        for (size_t i = 0; i < _player_fov_tile.field.size(); i++) {
            if (_core->_registry.valid(_player_fov_tile.field[i])) {
                const auto floor = _core->_registry.get<components::fields::Floor>(_player_fov_tile.field[i]);
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVFloor>(i)][floor.type].get(), 0, 0, WHITE);
            } else {
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVFloor>(i)][FloorType::RUINS_01].get(), 0, 0, RED);
            }
        }
        for (const assets::dungeon_view::POVWall i: draw_order_walls) {
            if (_core->_registry.valid(_player_fov_wall.field[static_cast<const size_t>(i)])) {
                const auto *wall = _core->_registry.try_get<components::fields::Wall>(_player_fov_wall.field[static_cast<const size_t>(i)]);
                if (wall) {
                    DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVWall>(i)][wall->type].get(), 0, 0, WHITE);
                }
            }
        }
    }
    EndTextureMode();
}
void DungeonView::_render_minimap() {
    BeginTextureMode(_render_texture_gui);
    ClearBackground(BACKGROUND_COLOR);
    DrawTexture(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Background].get(), 0, 0, WHITE);
    uint8_t offset_x = 10, offset_y = 10;
    for (auto tile: _tile_map._tiles) {
        if (_core->_registry.valid(tile.entity)) {
            components::fields::MapPosition position = _core->_registry.get<components::fields::MapPosition>(tile.entity);
            auto *tile_in_fov = _core->_registry.try_get<components::fields::InFovOfEntity>(tile.entity);
            if (tile_in_fov) {
                DrawRectangle(position.x * 5 + offset_x, position.y * 5 +offset_y, 5, 5, FOV_COLOR);
            }
            else {
                DrawRectangle(position.x * 5 + offset_x, position.y * 5 +offset_y, 5, 5, BACKGROUND_COLOR);
            }
        }
    }
    auto player_view = _core->_registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    player_view.each([&](const entt::entity entity, const components::general::Player player, components::general::Direction direction, components::fields::MapPosition position) {
        switch (direction.direction) {
            case WorldDirection::NORTH:
                DrawTextureRec(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get(), Rectangle{0,0,5,5}, Vector2{static_cast<float>(position.x * 5.0 + offset_x), static_cast<float>(position.y *5.0+offset_y)}, WHITE);
                break;
            case WorldDirection::EAST:
                DrawTextureRec(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get(), Rectangle{5,0,5,5}, Vector2{static_cast<float>(position.x * 5.0 + offset_x), static_cast<float>(position.y *5.0+offset_y)}, WHITE);
                break;
            case WorldDirection::SOUTH:
                DrawTextureRec(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get(), Rectangle{10,0,5,5}, Vector2{static_cast<float>(position.x * 5.0 + offset_x), static_cast<float>(position.y *5.0+offset_y)}, WHITE);
                break;
            case WorldDirection::WEST:
                DrawTextureRec(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get(), Rectangle{15,0,5,5}, Vector2{static_cast<float>(position.x * 5.0 + offset_x), static_cast<float>(position.y *5.0+offset_y)}, WHITE);
                break;
        }
    });
    for (WallEntity wall: _wall_map._walls) {
        components::fields::Wall wall_component = _core->_registry.get<components::fields::Wall>(wall.entity);
        components::fields::MapPosition field1_position = _core->_registry.get<components::fields::MapPosition>(wall_component.field1);
        components::fields::MapPosition field2_position = _core->_registry.get<components::fields::MapPosition>(wall_component.field2);
        if (field1_position.x == field2_position.x) {
            DrawLine(field1_position.x * 5 + offset_x, std::max(field1_position.y, field2_position.y) * 5 + offset_y, field1_position.x * 5 + offset_x + 5, std::max(field1_position.y, field2_position.y) * 5 + offset_y, WALL_COLOR);
        }
        else if (field1_position.y == field2_position.y) {
            DrawLine(std::max(field1_position.x, field2_position.x) * 5 + offset_x, field1_position.y * 5 + offset_y, std::max(field1_position.x, field2_position.x) * 5 + offset_x, field1_position.y * 5 + offset_y + 5, WALL_COLOR);
        }
    }
    EndTextureMode();
}
void DungeonView::render() {
    BeginDrawing();

    ClearBackground(BLACK);
    _render_pov();
    ClearBackground(BLACK);
    _render_minimap();
    DrawTexturePro(_render_texture_pov.texture,
                   Rectangle{0.0f, 0.0f, (float) _render_texture_pov.texture.width, (float) -_render_texture_pov.texture.height},
                   POV_DIMENSION,
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
    DrawTexturePro(_render_texture_gui.texture,
                   Rectangle{0.0f, 0.0f, (float) _render_texture_gui.texture.width, (float) -_render_texture_gui.texture.height},
                   GUI_DIMENSION,
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
    EndDrawing();
}

void DungeonView::update() {
    static bool after_first_update = false;
    static bool recalculate_fov = true;
    if (after_first_update && recalculate_fov) {
        _calculate_fov();
        recalculate_fov = false;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        _core->_registry.view<components::general::Player, components::general::Direction>().each([](const entt::entity entity, const components::general::Player player, components::general::Direction &direction) {
            switch (direction.direction) {
                case WorldDirection::NORTH:
                    direction.direction = WorldDirection::WEST;
                    break;
                case WorldDirection::EAST:
                    direction.direction = WorldDirection::NORTH;
                    break;
                case WorldDirection::SOUTH:
                    direction.direction = WorldDirection::EAST;
                    break;
                case WorldDirection::WEST:
                    direction.direction = WorldDirection::SOUTH;
                    break;
            }
        });
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        _core->_registry.view<components::general::Player, components::general::Direction>().each([](const entt::entity entity, const components::general::Player player, components::general::Direction &direction) {
            switch (direction.direction) {
                case WorldDirection::NORTH:
                    direction.direction = WorldDirection::EAST;
                    break;
                case WorldDirection::EAST:
                    direction.direction = WorldDirection::SOUTH;
                    break;
                case WorldDirection::SOUTH:
                    direction.direction = WorldDirection::WEST;
                    break;
                case WorldDirection::WEST:
                    direction.direction = WorldDirection::NORTH;
                    break;
            }
        });
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_UP)) {
        auto view = _core->_registry.view<components::general::Player, components::fields::MapPosition, components::general::Direction>();
        for (auto entity: view) {
            auto &position = _core->_registry.get<components::fields::MapPosition>(entity);
            auto direction = _core->_registry.get<components::general::Direction>(entity);
            switch (direction.direction) {
                case WorldDirection::NORTH: {
                    entt::entity destination = _tile_map.get_at(position.x, position.y - 1);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
                case WorldDirection::SOUTH: {
                    entt::entity destination = _tile_map.get_at(position.x, position.y + 1);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
                case WorldDirection::WEST: {
                    entt::entity destination = _tile_map.get_at(position.x - 1, position.y);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
                case WorldDirection::EAST: {
                    entt::entity destination = _tile_map.get_at(position.x + 1, position.y);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
            }
        }
        recalculate_fov = true;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        auto view = _core->_registry.view<components::general::Player, components::fields::MapPosition, components::general::Direction>();
        for (auto entity: view) {
            auto &position = _core->_registry.get<components::fields::MapPosition>(entity);
            auto direction = _core->_registry.get<components::general::Direction>(entity);
            switch (direction.direction) {
                case WorldDirection::NORTH: {
                    entt::entity destination = _tile_map.get_at(position.x, position.y + 1);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
                case WorldDirection::SOUTH: {
                    entt::entity destination = _tile_map.get_at(position.x, position.y - 1);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
                case WorldDirection::WEST: {
                    entt::entity destination = _tile_map.get_at(position.x + 1, position.y);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
                case WorldDirection::EAST: {
                    entt::entity destination = _tile_map.get_at(position.x - 1, position.y);
                    if (_core->_registry.valid(destination) && _core->_registry.get<components::fields::Walkability>(destination).is_walkable) {
                        auto destination_position = _core->_registry.get<components::fields::MapPosition>(destination);
                        position.x = destination_position.x;
                        position.y = destination_position.y;
                    }
                    break;
                }
            }
        }
        recalculate_fov = true;
    }
    after_first_update = true;
}

static entt::entity find_wall_between(const entt::entity field01, const entt::entity field02, const WallMap &wall_map) {
    return wall_map.get_between(field01, field02);
}

template<size_t AMOUNT_WALLS_IN_FOV>
static void fill_player_fov_walls(std::array<entt::entity, AMOUNT_WALLS_IN_FOV> &player_fov_walls, const components::fields::MapPosition player_position, const WallMap &wall_map, const TileMap &tile_map, const WorldDirection direction) {
    int x_mod, y_mod;
    switch (direction) {
        case WorldDirection::NORTH:
        case WorldDirection::EAST:
            x_mod = 1;
            y_mod = 1;
            break;
        case WorldDirection::SOUTH:
        case WorldDirection::WEST:
            x_mod = -1;
            y_mod = -1;
            break;
    }
    // 01 - 05
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 5 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 5 * x_mod, player_position.y - 2 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 5 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 5 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x, player_position.y - 5 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y), tile_map.get_at(player_position.x + 5 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 5 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod), tile_map.get_at(player_position.x + 5 * x_mod, player_position.y  + 1* y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y  + 1* y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y +1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_N] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 5 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod), tile_map.get_at(player_position.x + 5 * x_mod, player_position.y + 2 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod), wall_map);
    // 06 - 10
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W06_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W08_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y + 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W10_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod), wall_map);
    // 11 - 13
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W12_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y + 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y + 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y + 1* y_mod), wall_map);
    // 14 - 16
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W15_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x, player_position.y), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y), tile_map.get_at(player_position.x, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y + 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y + 1 * y_mod), tile_map.get_at(player_position.x, player_position.y + 1 * y_mod), wall_map);
    // 17 - 19
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W17_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y), tile_map.get_at(player_position.x, player_position.y), wall_map) : find_wall_between(tile_map.get_at(player_position.x, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x, player_position.y), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W19_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x, player_position.y), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y), wall_map) : find_wall_between(tile_map.get_at(player_position.x, player_position.y), tile_map.get_at(player_position.x, player_position.y + 1 * y_mod), wall_map);
}
template <size_t SIZE>
static void fill_player_fov_tiles(std::array<entt::entity, SIZE> &player_fov_tiles, const components::fields::MapPosition player_position, const TileMap &tile_map, const WorldDirection direction) {
    int x_mod, y_mod;
    switch (direction) {
        case WorldDirection::NORTH:
        case WorldDirection::EAST:
            x_mod = 1;
            y_mod = 1;
            break;
        case WorldDirection::SOUTH:
        case WorldDirection::WEST:
            x_mod = -1;
            y_mod = -1;
            break;
    }

    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F01] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F02] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F03] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F04] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F05] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F06] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F07] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F08] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F09] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F10] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F11] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F12] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F13] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F14] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F15] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F16] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F17] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y) : tile_map.get_at(player_position.x, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F18] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y) : tile_map.get_at(player_position.x, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F19] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y) : tile_map.get_at(player_position.x, player_position.y + 1 * y_mod);
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
