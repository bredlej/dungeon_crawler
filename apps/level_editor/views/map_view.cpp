//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <map_view.hpp>

void MapView::render() noexcept {
    using namespace editor;
    ClearBackground(BLACK);
    _draw_grid();
    _draw_tile_map();
    _draw_wall_map();
    _draw_cursor();
}

void MapView::set_edit_mode(ChangeEditMode change_edit_mode) {
    _core->registry.ctx().erase<CurrentEditMode>();
    _core->registry.ctx().emplace<CurrentEditMode>(change_edit_mode.edit_mode);
}
void MapView::load_level(LoadLevel level) {
    _core->registry.ctx().erase<CurrentEditMode>();
    _core->registry.ctx().emplace<CurrentEditMode>(EditMode::None);
    _core->registry.ctx().erase<LevelFileName>();
    _core->registry.ctx().emplace<LevelFileName>(level.path.filename());
    _level.load(level.path);
}
void MapView::update() noexcept {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (_mouse_drag_start_position.x == 0 && _mouse_drag_start_position.y == 0)
        {
            _mouse_drag_start_position = GetMousePosition();
        }
        else
        {
            _mouse_drag_end_position = GetMousePosition();
        }
    }
    else {
        _mouse_drag_start_position = _mouse_drag_end_position = (Vector2){0, 0};
    }
}

void MapView::save_level(const SaveLevel &level) {
    try {
        _level.save("../../assets/levels/" + level.path);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    _core->dispatcher.enqueue<RefreshLevels>();
}

void MapView::_check_tile_collision(const int32_t x, const int32_t y) const {
    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size})) {
        DrawRectangleLines(x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size, GREEN);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            _core->registry.ctx().erase<EntitySelected>();
            _core->registry.ctx().emplace<editor::EntitySelected>(_level.tile_map.get_at(x, y));
        }
    }
}
void MapView::_check_wall_collision(int32_t x, int32_t y) const {
    static constexpr float divider = 5.0f;
    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x, y * _cell_size + _offset.y - (_cell_size / divider), _cell_size, 2 * (_cell_size / divider)})) {
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y - 1, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y - 1, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y + 1, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y + 1, RED);
    }
    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x - (_cell_size / divider), y * _cell_size + _offset.y, 2 * (_cell_size / divider), _cell_size})) {
        DrawLine(x * _cell_size + _offset.x - 1, y * _cell_size + _offset.y, x * _cell_size + _offset.x - 1, y * _cell_size + _offset.y + _cell_size, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y, x * _cell_size + _offset.x, y * _cell_size + _offset.y + _cell_size, RED);
        DrawLine(x * _cell_size + _offset.x + 1, y * _cell_size + _offset.y, x * _cell_size + _offset.x + 1, y * _cell_size + _offset.y + _cell_size, RED);
    }
}

void MapView::new_level(NewLevel new_level) {
    _level.clear();
    _level.new_level(new_level.width, new_level.height);
}
void MapView::_draw_grid() const {
    static int start_selection_x = 0;
    static int start_selection_y = 0;
    static int end_selection_x = 0;
    static int end_selection_y = 0;
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            DrawRectangleLines(x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size, palette::gray);
            if (_mouse_drag_start_position.x >= x * _cell_size + _offset.x && _mouse_drag_start_position.x <= x * _cell_size + _offset.x + _cell_size && _mouse_drag_start_position.y >= y * _cell_size + _offset.y && _mouse_drag_start_position.y <= y * _cell_size + _offset.y + _cell_size) {
                start_selection_x = x;
                start_selection_y = y;
                std::printf("(%d, %d)\n",x,y);
            }
            if (_mouse_drag_end_position.x >= x * _cell_size + _offset.x && _mouse_drag_end_position.x <= x * _cell_size + _offset.x + _cell_size && _mouse_drag_end_position.y >= y * _cell_size + _offset.y && _mouse_drag_end_position.y <= y * _cell_size + _offset.y + _cell_size) {
                end_selection_x = x;
                end_selection_y = y;
                std::printf("END (%d, %d)\n",x,y);
            }
        }
    }
    if (_mouse_drag_start_position.x != 0 && _mouse_drag_start_position.y != 0 && _mouse_drag_end_position.x != 0 && _mouse_drag_end_position.y != 0)
    {
     //  DrawRectangleLines(start_selection_x * _cell_size + _offset.x, start_selection_y * _cell_size + _offset.y, (end_selection_x-start_selection_x + 1) * _cell_size, (end_selection_y-start_selection_y+1)* _cell_size, palette::yellow);
        const auto min_x = std::min(start_selection_x, end_selection_x);
        const auto min_y = std::min(start_selection_y, end_selection_y);
        const auto max_x = std::max(start_selection_x, end_selection_x);
        const auto max_y = std::max(start_selection_y, end_selection_y);
        DrawRectangleLinesEx({min_x * _cell_size + _offset.x, min_y * _cell_size + _offset.y, (max_x-min_x + 1) * _cell_size, (max_y-min_y+1)* _cell_size}, 5, palette::yellow);
    }
}

void MapView::_draw_tile_map() const {
    for (const auto &tile: _level.tile_map._tiles) {
        components::fields::MapPosition position = _core->registry.get<components::fields::MapPosition>(tile.entity);
        if (tile.entity == entt::null) {
            DrawRectangle(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, GRAY);
        }
        if (_core->registry.any_of<components::fields::Floor>(tile.entity)) {
            components::fields::Floor floor = _core->registry.get<components::fields::Floor>(tile.entity);
            DrawTextureRec(_core->registry.ctx().find<EditorAssets>()->_textures[floor.type].get(), {0, 0, 25, 25}, {position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y}, WHITE);
            DrawRectangleLines(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, palette::dark_gray);
        }
    }
}
void MapView::_draw_wall_map() const {
    for (const auto &wall: _level.wall_map._walls) {
        const components::fields::Wall wall_data = _core->registry.get<components::fields::Wall>(wall.entity);
        const components::fields::MapPosition field1 = wall_data.field1;
        const components::fields::MapPosition field2 = wall_data.field2;
        if (field1.x < field2.x && field1.y == field2.y) {
            DrawLine(field2.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y, field2.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y + _cell_size, palette::yellow);
        } else if (field1.x > field2.x && field1.y == field2.y) {
            DrawLine(field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y, field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y + _cell_size, palette::yellow);
        } else if (field1.x == field2.x && field1.y < field2.y) {
            DrawLine(field1.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y, field1.x * _cell_size + _offset.x + _cell_size, field2.y * _cell_size + _offset.y, palette::yellow);
        } else if (field1.x == field2.x && field1.y > field2.y) {
            DrawLine(field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y, field1.x * _cell_size + _offset.x + _cell_size, field1.y * _cell_size + _offset.y, palette::yellow);
        }
    }
}
void MapView::_draw_cursor() const {
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            switch (_core->registry.ctx().find<CurrentEditMode>()->edit_mode) {
                case EditMode::None:
                    break;
                case EditMode::Tile:
                    _check_tile_collision(x, y);
                    break;
                case EditMode::Wall:
                    _check_wall_collision(x, y);
                    break;
                default:
                    break;
            }
        }
    }
}
