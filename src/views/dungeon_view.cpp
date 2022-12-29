//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>
void DungeonView::render() {
    using namespace assets::dungeon_view;
    BeginDrawing();
    ClearBackground(BLACK);

    BeginTextureMode(_render_texture);
    ClearBackground(BLACK);
    if (auto *assets = _core->get_assets()) {
        for (int8_t i = 0; i < _player_fov.size(); i++) {
            if (_core->_registry.valid(_player_fov[i])) {
                const auto floor = _core->_registry.get<components::fields::Floor>(_player_fov[i]);
                const auto ceiling = _core->_registry.get<components::fields::Ceiling>(_player_fov[i]);
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][floor.type].get(), 0, 0, WHITE);
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][ceiling.type].get(), 0, 0, WHITE);

            } else {
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][FloorType::NORMAL].get(), 0, 0, RED);
                DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][CeilingType::NORMAL].get(), 0, 0, RED);
            }
        }
        // clang-format off
        constexpr static std::array<assets::dungeon_view::POVField, 19> draw_order = {
                POVField::F05, POVField::F04, POVField::F01, POVField::F02, POVField::F03,
                POVField::F10, POVField::F09, POVField::F06, POVField::F07, POVField::F08,
                         POVField::F13, POVField::F11, POVField::F12,
                         POVField::F16, POVField::F14, POVField::F15,
                         POVField::F19, POVField::F17, POVField::F18,
        };
        // clang-format on
        for (assets::dungeon_view::POVField i: draw_order) {
            if (_core->_registry.valid(_player_fov[static_cast<const size_t>(i)])) {
                const auto *wall = _core->_registry.try_get<components::fields::Wall>(_player_fov[static_cast<const size_t>(i)]);
                if (wall) {
                    DrawTexture(assets->_textures._tiles[static_cast<assets::dungeon_view::POVField>(i)][wall->type].get(), 0, 0, WHITE);
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

static void fill_player_fov(std::vector<entt::entity> &player_fov, const components::fields::MapPosition player_position, const TileMap& tile_map, WorldDirection direction) {
    int x_mod, y_mod;
    switch (direction) {
        case WorldDirection::NORTH:
        case WorldDirection::EAST:
            x_mod = 1; y_mod = 1; break;
        case WorldDirection::SOUTH:
        case WorldDirection::WEST:
            x_mod = -1; y_mod = -1; break;
    }

    player_fov[(size_t) assets::dungeon_view::POVField::F01] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 2 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F02] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y - 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F03] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 4 * y_mod)             : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y)    ;
    player_fov[(size_t) assets::dungeon_view::POVField::F04] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F05] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 4 * y_mod) : tile_map.get_at(player_position.x + 4 * x_mod, player_position.y + 2 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F06] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 2 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 2 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F07] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y - 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F08] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 3 * y_mod)             : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y)    ;
    player_fov[(size_t) assets::dungeon_view::POVField::F09] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F10] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 3 * y_mod) : tile_map.get_at(player_position.x + 3 * x_mod, player_position.y + 2 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F11] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y - 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F12] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 2 * y_mod)             : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y)    ;
    player_fov[(size_t) assets::dungeon_view::POVField::F13] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 2 * y_mod) : tile_map.get_at(player_position.x + 2 * x_mod, player_position.y + 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F14] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F15] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y - 1 * y_mod)             : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y)    ;
    player_fov[(size_t) assets::dungeon_view::POVField::F16] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y - 1 * y_mod) : tile_map.get_at(player_position.x + 1 * x_mod, player_position.y + 1 * y_mod);
    player_fov[(size_t) assets::dungeon_view::POVField::F17] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x - 1 * x_mod, player_position.y)             : tile_map.get_at(player_position.x, player_position.y - 1 * y_mod)    ;
    player_fov[(size_t) assets::dungeon_view::POVField::F18] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x, player_position.y)                         : tile_map.get_at(player_position.x, player_position.y);
    player_fov[(size_t) assets::dungeon_view::POVField::F19] = (direction == WorldDirection::NORTH || direction == WorldDirection::SOUTH) ? tile_map.get_at(player_position.x + 1 * x_mod, player_position.y)             : tile_map.get_at(player_position.x, player_position.y + 1 * y_mod)    ;
}

void DungeonView::_calculate_fov() {
    auto player_view = _core->_registry.view<components::general::Player, components::general::Direction, components::fields::MapPosition>();
    for (auto entity: player_view) {
        auto player_position = player_view.get<components::fields::MapPosition>(entity);
        auto player_direction = player_view.get<components::general::Direction>(entity);

        fill_player_fov(_player_fov, player_position, _tile_map, player_direction.direction);
    }
}
