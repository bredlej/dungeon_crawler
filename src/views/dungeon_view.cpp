//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>

constexpr static Color BackgroundColor = {0x34, 0x31, 0x1d, 0xff};

void DungeonView::render() {
    using namespace assets::dungeon_view;
    BeginDrawing();

    ClearBackground(BLACK);

    BeginTextureMode(_render_texture);
    ClearBackground(BackgroundColor);
    if (auto *assets = _core->get_assets()) {
        for (int8_t i = 0; i < _player_fov_tile.size(); i++) {
            if (_core->_registry.valid(_player_fov_tile[i])) {
                const auto floor = _core->_registry.get<components::fields::Floor>(_player_fov_tile[i]);
                // const auto ceiling = _core->_registry.get<components::fields::Ceiling>(_player_fov[i]);
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][floor.type].get(), 0, 0, WHITE);
                //  DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][ceiling.type].get(), 0, 0, WHITE);

            } else {
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][FloorType::RUINS_01].get(), 0, 0, RED);
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][CeilingType::NORMAL].get(), 0, 0, RED);
            }
        }
        // clang-format off
        constexpr static std::array<assets::dungeon_view::POVField, 19> draw_order_floor = {
                POVField::F05, POVField::F04, POVField::F01, POVField::F02, POVField::F03,
                POVField::F10, POVField::F09, POVField::F06, POVField::F07, POVField::F08,
                         POVField::F13, POVField::F11, POVField::F12,
                         POVField::F16, POVField::F14, POVField::F15,
                         POVField::F19, POVField::F17, POVField::F18,
        };

        constexpr static std::array<assets::dungeon_view::POVWall, 28> draw_order_walls = {
            POVWall::W01_E, POVWall::W02_E, POVWall::W04_W, POVWall::W05_W, POVWall::W01_S, POVWall::W02_S, POVWall::W03_S, POVWall::W04_S, POVWall::W05_S,
            POVWall::W06_E, POVWall::W07_E, POVWall::W09_W, POVWall::W10_W, POVWall::W07_S, POVWall::W08_S, POVWall::W09_S,
            POVWall::W11_E, POVWall::W13_W, POVWall::W11_S, POVWall::W12_S, POVWall::W13_S,
            POVWall::W14_E, POVWall::W16_W, POVWall::W14_S, POVWall::W15_S, POVWall::W16_S,
            POVWall::W17_E, POVWall::W19_W
        };
        // clang-format on
        for (const assets::dungeon_view::POVWall i: draw_order_walls) {
            if (_core->_registry.valid(_player_fov_wall[static_cast<const size_t>(i)])) {
                const auto *wall = _core->_registry.try_get<components::fields::Wall>(_player_fov_wall[static_cast<const size_t>(i)]);
                if (wall) {
                    DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVWall>(i)][wall->type].get(), 0, 0, WHITE);
                }
            }
        }
    }
    EndTextureMode();
    DrawTexturePro(_render_texture.texture,
                   Rectangle{0.0f, 0.0f, (float) _render_texture.texture.width, (float) -_render_texture.texture.height},
                   Rectangle{15.0f, 15.0f, static_cast<float>(640.0f), static_cast<float>(480.0f)},
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
    EndDrawing();
}

void DungeonView::update() {
    _calculate_fov();
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
    }
}

static entt::entity find_wall_between(const entt::entity field01, const entt::entity field02, const WallMap &wall_map) {
    return wall_map.get_between(field01, field02);
}

static void fill_player_fov_walls(std::vector<entt::entity> &player_fov_walls, const components::fields::MapPosition player_position, const WallMap &wall_map, const TileMap &tile_map, const WorldDirection direction) {
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
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y +1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod), tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod), wall_map);
    // 06 - 10
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W06_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W08_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y +1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W10_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod), tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod), wall_map);
    // 11 - 13
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W12_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 2 * x_mod, player_position.y +1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 2 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), wall_map);
    // 14 - 16
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x - 1 * x_mod, player_position.y * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W15_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y +1 * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_S] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), wall_map);
    // 17 - 19
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W17_E] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x - 1 * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y - 1 * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), wall_map);
    player_fov_walls[(size_t) assets::dungeon_view::POVWall::W19_W] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x + 1 * x_mod, player_position.y * y_mod), wall_map) : find_wall_between(tile_map.get_at(player_position.x * x_mod, player_position.y * y_mod), tile_map.get_at(player_position.x * x_mod, player_position.y +1 * y_mod), wall_map);

}

static void fill_player_fov_tiles(std::vector<entt::entity> &player_fov_tiles, const components::fields::MapPosition player_position, const TileMap &tile_map, const WorldDirection direction) {
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

    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F01] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F02] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F03] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F04] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F05] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F06] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F07] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F08] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F09] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F10] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F11] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F12] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F13] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F14] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F15] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F16] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y + 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F17] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y) : tile_map.get_at(player_position.x, player_position.y - 1 * y_mod);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F18] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y) : tile_map.get_at(player_position.x, player_position.y);
    player_fov_tiles[(size_t) assets::dungeon_view::POVField::F19] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y) : tile_map.get_at(player_position.x, player_position.y + 1 * y_mod);
}

void DungeonView::_calculate_fov() {
    auto player_view = _core->_registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    for (auto entity: player_view) {
        auto player_position = player_view.get<components::fields::MapPosition>(entity);
        auto player_direction = player_view.get<components::general::Direction>(entity);

        fill_player_fov_tiles(_player_fov_tile, player_position, _tile_map, player_direction.direction);
        fill_player_fov_walls(_player_fov_wall, player_position, _wall_map, _tile_map, player_direction.direction);
    }
}
