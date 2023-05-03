//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <map_view.hpp>

static inline bool is_tile_select(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Tile && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Select;
}
void MapView::render() noexcept {
    using namespace editor;
    ClearBackground(BLACK);
    _draw_grid();
    _draw_tile_map();
    _draw_wall_map();
    _draw_cursor();
    _draw_selection_hovered_tile();
    if (is_tile_select(*_core)) {
        _select_tiles_in_rectangle();
    }
}

void MapView::load_level(LoadLevel level) {
    _core->dispatcher.enqueue<ClearSelection>();
    _core->registry.ctx().erase<CurrentEditMode>();
    _core->registry.ctx().emplace<CurrentEditMode>(EditMode::None);
    _core->registry.ctx().erase<LevelFileName>();
    _core->registry.ctx().emplace<LevelFileName>(level.path.filename());
    _core->registry.ctx().erase<EntitiesSelected>();
    _level.load(level.path);
}

void MapView::update() noexcept {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (_mouse_drag_start_position.x == 0 && _mouse_drag_start_position.y == 0) {
            _mouse_drag_start_position = GetMousePosition();
        } else {
            _mouse_drag_end_position = GetMousePosition();
        }
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && is_tile_select(*_core)) {
        _core->dispatcher.enqueue<ClearSelection>();
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
    const auto *edit_mode_tool = _core->registry.ctx().find<CurrentEditModeTool>();

    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x, y * _cell_size + _offset.y - (_cell_size / divider), _cell_size, 2 * (_cell_size / divider)})) {
        // upper
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y - 1, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y - 1, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y + 1, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y + 1, RED);
        if (edit_mode_tool && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            switch (edit_mode_tool->edit_mode_tool) {
                case EditModeTool::Select:
                    _core->dispatcher.enqueue<WallSelected>(MapPosition{x,y-1}, MapPosition{x, y});
                    break;
                case EditModeTool::Paint:
                    _core->dispatcher.enqueue<WallAdded>(WallType::RUINS_01, MapPosition{x,y-1}, MapPosition{x, y});
                    break;
                default:
                    break;
            }
        }
        else if (edit_mode_tool && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            switch (edit_mode_tool->edit_mode_tool) {
                case EditModeTool::Select:
                    _core->registry.ctx().erase<WallSelected>();
                    break;
                case EditModeTool::Paint:
                    _core->dispatcher.enqueue<WallRemoved>(MapPosition{x, y-1}, MapPosition{x, y});
                    break;
                default:
                    break;
            }
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x - (_cell_size / divider), y * _cell_size + _offset.y, 2 * (_cell_size / divider), _cell_size})) {
        // left
        DrawLine(x * _cell_size + _offset.x - 1, y * _cell_size + _offset.y, x * _cell_size + _offset.x - 1, y * _cell_size + _offset.y + _cell_size, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y, x * _cell_size + _offset.x, y * _cell_size + _offset.y + _cell_size, RED);
        DrawLine(x * _cell_size + _offset.x + 1, y * _cell_size + _offset.y, x * _cell_size + _offset.x + 1, y * _cell_size + _offset.y + _cell_size, RED);
        if (edit_mode_tool && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            switch (edit_mode_tool->edit_mode_tool) {
                case EditModeTool::Select:
                    _core->dispatcher.enqueue<WallSelected>(MapPosition{x-1, y}, MapPosition{x, y});
                    break;
                case EditModeTool::Paint:
                    _core->dispatcher.enqueue<WallAdded>(WallType::RUINS_01, MapPosition{x-1,y}, MapPosition{x, y});
                    break;
                default:
                    break;
            }
        }
        else if (edit_mode_tool && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            switch (edit_mode_tool->edit_mode_tool) {
                case EditModeTool::Select:
                    _core->registry.ctx().erase<WallSelected>();
                    break;
                case EditModeTool::Paint:
                    _core->dispatcher.enqueue<WallRemoved>(MapPosition{x-1,y}, MapPosition{x, y});
                    break;
                default:
                    break;
            }
        }
    }
}

void MapView::new_level(NewLevel new_level) {
    _level.clear();
    _level.new_level(new_level.width, new_level.height);
}
void MapView::_draw_grid() const {
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            DrawRectangleLines(x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size, palette::gray);
        }
    }
}
void MapView::_draw_selection_hovered_tile() const {
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            if (auto *position_hovered = _core->registry.ctx().find<MapPositionHovered>() ) {
                if (position_hovered->position.x == x && position_hovered->position.y == y) {
                    DrawRectangleLinesEx({x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size}, 3, palette::yellow);
                }
            }
        }
    }
}

void MapView::_draw_tile_map() const {
    for (const auto &tile: _level.tile_map._tiles) {
        components::fields::MapPosition position = _core->registry.get<components::fields::MapPosition>(tile.entity);
        if (tile.entity == entt::null) {
            DrawRectangle(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, GRAY);
        }
        else {
            if (_core->registry.any_of<components::fields::Floor>(tile.entity)) {
                components::fields::Floor floor = _core->registry.get<components::fields::Floor>(tile.entity);
                DrawTextureRec(_core->registry.ctx().find<EditorAssets>()->_textures[floor.type].get(), {0, 0, 25, 25}, {position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y}, WHITE);
                DrawRectangleLines(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, palette::dark_gray);
            }
            else {
                DrawRectangle(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, palette::dark_gray);
            }
        }
    }
    if (auto *rectangle_selected = _core->registry.ctx().find<RectangleSelected>()) {
        DrawRectangleLines(rectangle_selected->selection.x, rectangle_selected->selection.y, rectangle_selected->selection.width, rectangle_selected->selection.height, RED);
    }
}
void MapView::_draw_wall_map() const {
    const auto *wall_selected = _core->registry.ctx().find<WallSelected>();
    for (const auto &wall: _level.wall_map._walls) {
        const components::fields::Wall wall_data = _core->registry.get<components::fields::Wall>(wall.entity);
        const components::fields::MapPosition field1 = wall_data.field1;
        const components::fields::MapPosition field2 = wall_data.field2;
        static constexpr int wall_thickness = 3;
        static constexpr Color wall_color = palette::light_brown;
        if (field1.x < field2.x && field1.y == field2.y) {
            DrawLineEx({field2.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y}, {field2.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y + _cell_size}, wall_thickness, wall_color);
        } else if (field1.x > field2.x && field1.y == field2.y) {
            DrawLineEx({field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y}, {field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y + _cell_size}, wall_thickness, wall_color);
        } else if (field1.x == field2.x && field1.y < field2.y) {
            DrawLineEx({field1.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y}, {field1.x * _cell_size + _offset.x + _cell_size, field2.y * _cell_size + _offset.y}, wall_thickness, wall_color);
        } else if (field1.x == field2.x && field1.y > field2.y) {
            DrawLineEx({field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y}, {field1.x * _cell_size + _offset.x + _cell_size, field1.y * _cell_size + _offset.y}, wall_thickness, wall_color);
        }
    }
    if (wall_selected) {
        if (wall_selected->position1.x < wall_selected->position2.x && wall_selected->position1.y == wall_selected->position2.y) {
            DrawLineEx({wall_selected->position2.x * _cell_size + _offset.x, wall_selected->position1.y * _cell_size + _offset.y}, {wall_selected->position2.x * _cell_size + _offset.x, wall_selected->position2.y * _cell_size + _offset.y + _cell_size}, 3, palette::yellow);
        }
        else if (wall_selected->position1.x == wall_selected->position2.x && wall_selected->position1.y < wall_selected->position2.y) {
            DrawLineEx({wall_selected->position1.x * _cell_size + _offset.x, wall_selected->position2.y * _cell_size + _offset.y}, {wall_selected->position2.x * _cell_size + _offset.x + _cell_size, wall_selected->position2.y * _cell_size + _offset.y}, 3, palette::yellow);
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
void MapView::_select_tiles_in_rectangle() {
    _selected_positions.clear();
    static int start_selection_x = 0;
    static int start_selection_y = 0;
    static int end_selection_x = 0;
    static int end_selection_y = 0;
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            if (_mouse_drag_start_position.x > _level.tile_map._width * _cell_size + _offset.x || _mouse_drag_start_position.y > _level.tile_map._height * _cell_size + _offset.y || _mouse_drag_end_position.x > _level.tile_map._width * _cell_size + _offset.x || _mouse_drag_end_position.y > _level.tile_map._height * _cell_size + _offset.y) {
                return;
            }
            if (_mouse_drag_start_position.x >= x * _cell_size + _offset.x && _mouse_drag_start_position.x <= x * _cell_size + _offset.x + _cell_size && _mouse_drag_start_position.y >= y * _cell_size + _offset.y && _mouse_drag_start_position.y <= y * _cell_size + _offset.y + _cell_size) {
                start_selection_x = x;
                start_selection_y = y;
            }
            if (_mouse_drag_end_position.x >= x * _cell_size + _offset.x && _mouse_drag_end_position.x <= x * _cell_size + _offset.x + _cell_size && _mouse_drag_end_position.y >= y * _cell_size + _offset.y && _mouse_drag_end_position.y <= y * _cell_size + _offset.y + _cell_size) {
                end_selection_x = x;
                end_selection_y = y;
            }
        }
    }
    if (_mouse_drag_start_position.x != 0 && _mouse_drag_start_position.y != 0 && _mouse_drag_end_position.x != 0 && _mouse_drag_end_position.y != 0) {
        const auto min_x = std::min(start_selection_x, end_selection_x);
        const auto min_y = std::min(start_selection_y, end_selection_y);
        const auto max_x = std::max(start_selection_x, end_selection_x);
        const auto max_y = std::max(start_selection_y, end_selection_y);
        std::vector<components::fields::MapPosition> selected_positions;
        for (int32_t i = min_y; i <= max_y; i++) {
            for (int32_t j = min_x; j <= max_x; j++) {
                selected_positions.emplace_back(components::fields::MapPosition{j, i});
            }
        }
        _core->dispatcher.enqueue<MapPositionSelected>(selected_positions);
        _core->registry.ctx().erase<MapPositionSelected>();
        _core->registry.ctx().emplace<MapPositionSelected>(selected_positions);
        Rectangle selection = {min_x * _cell_size + _offset.x, min_y * _cell_size + _offset.y, (max_x - min_x + 1) * _cell_size, (max_y - min_y + 1) * _cell_size};
        _core->registry.ctx().erase<RectangleSelected>();
        _core->registry.ctx().emplace<RectangleSelected>(selection);
        DrawRectangleLinesEx(selection, 3, palette::yellow);
    }
}

void MapView::_handle_entities_selection(const MapPositionSelected &event) {
    _core->registry.ctx().erase<EntitiesSelected>();
    std::vector<std::pair<components::fields::MapPosition, entt::entity>> entities;
    for (const components::fields::MapPosition &position: event.positions) {
        const auto entity = _level.tile_map.get_at(position.x, position.y);
        entities.emplace_back(position, entity);
    }
    _core->registry.ctx().emplace<EntitiesSelected>(entities);
}

void MapView::_clear_rectangle_selection(ClearSelection clearSelection) {
    _core->registry.ctx().erase<RectangleSelected>();
    _core->registry.ctx().erase<EntitiesSelected>();
}

void MapView::_place_floor(editor::PlaceComponent<Floor> event) {
    for (const auto &position: *event.positions) {
        auto field = _level.tile_map.get_at(position.x, position.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::fields::Field>(field);
            _core->registry.emplace<components::fields::MapPosition>(field, position.x, position.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
        _core->registry.emplace_or_replace<components::fields::Floor>(field, event.component.type);
    }
}
void MapView::_place_encounter_chance(editor::PlaceComponent<EncounterChance> event) {
    for (const auto &position: *event.positions) {
        auto field = _level.tile_map.get_at(position.x, position.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::fields::Field>(field);
            _core->registry.emplace<components::fields::MapPosition>(field, position.x, position.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
        _core->registry.emplace_or_replace<components::values::EncounterChance>(field, event.component.chance);
    }
}

void MapView::_place_walkability(editor::PlaceComponent<Walkability> event) {
    for (const auto &position: *event.positions) {
        auto field = _level.tile_map.get_at(position.x, position.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::fields::Field>(field);
            _core->registry.emplace<components::fields::MapPosition>(field, position.x, position.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
        _core->registry.emplace_or_replace<components::fields::Walkability>(field, event.component.walkable);
    }
}

void MapView::_remove_all_selected_tiles() {
    const auto *selected_tiles = _core->registry.ctx().find<EntitiesSelected>();
    for (const auto &tile: selected_tiles->entities) {
        if (_core->registry.valid(tile.second) && tile.second != entt::null) {
            _core->registry.destroy(tile.second);
        }
    }
    _level.tile_map._tiles.erase(std::remove_if(_level.tile_map._tiles.begin(), _level.tile_map._tiles.end(), [selected_tiles](const Tile &tile) {
        for (const auto &selected_tile: selected_tiles->entities) {
            if (selected_tile.second == tile.entity) {
                return true;
            }
        }
        return false;
    }), _level.tile_map._tiles.end());
    _core->registry.ctx().erase<EntitiesSelected>();

    auto *selected_positions = _core->registry.ctx().find<MapPositionSelected>();
    if (selected_positions) {
        _core->dispatcher.enqueue<MapPositionSelected>(selected_positions->positions);
    }
}

void MapView::_select_wall(const WallSelected &wallSelected) {
    _core->registry.ctx().erase<WallSelected>();
    _core->registry.ctx().emplace<WallSelected>(wallSelected);
}

void MapView::_add_wall(const WallAdded &wallAdded) {
    bool has_wall = false;
    _core->registry.view<Wall>().each([this, &wallAdded, &has_wall](const entt::entity &entity, const Wall &wall) {
        if (wall.field1.x == wallAdded.position1.x && wall.field1.y == wallAdded.position1.y && wall.field2.x == wallAdded.position2.x && wall.field2.y == wallAdded.position2.y) {
            has_wall = true;
            _core->registry.emplace_or_replace<components::fields::Wall>(entity, wallAdded.type, wallAdded.position1, wallAdded.position2);
        }
    });
    if (!has_wall) {
        auto wall = _core->registry.create();
        _core->registry.emplace<components::fields::Wall>(wall, wallAdded.type, wallAdded.position1, wallAdded.position2);
        _level.wall_map._walls.emplace_back(WallEntity{wall});
    }
}

void MapView::_remove_wall(const WallRemoved &wallRemoved) {
    entt::entity to_remove = entt::null;
    _core->registry.view<Wall>().each([this, &wallRemoved, &to_remove](const entt::entity &entity, const Wall &wall) {
        if (wall.field1.x == wallRemoved.position1.x && wall.field1.y == wallRemoved.position1.y && wall.field2.x == wallRemoved.position2.x && wall.field2.y == wallRemoved.position2.y) {
            to_remove = entity;
        }
    });
    if (to_remove != entt::null) {
        _level.wall_map._walls.erase(std::remove_if(_level.wall_map._walls.begin(), _level.wall_map._walls.end(), [to_remove](const WallEntity &wall) {
            return wall.entity == to_remove;
        }), _level.wall_map._walls.end());
        _core->registry.destroy(to_remove);
    }
}
