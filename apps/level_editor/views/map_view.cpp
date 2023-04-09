//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <map_view.hpp>

void MapView::render() {
    using namespace editor;
    ClearBackground(BLACK);
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            DrawRectangleLines(x * _spacing + _offset.x, y * _spacing + _offset.y, _spacing, _spacing, GRAY);
        }
    }
    for (const auto &tile: _level.tile_map._tiles) {
        components::fields::MapPosition position = _core->registry.get<components::fields::MapPosition>(tile.entity);
        if (tile.entity == entt::null) {
            DrawRectangle(position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing, GRAY);
        }
        if (_core->registry.any_of<components::fields::Floor>(tile.entity)) {
            components::fields::Floor floor = _core->registry.get<components::fields::Floor>(tile.entity);
            if (floor.type == FloorType::RUINS_01) {
                DrawRectangle(position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing, ORANGE);
            }
            else {
                DrawRectangle(position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing, YELLOW);
            }
        }
        switch (_core->registry.ctx().find<CurrentEditMode>()->edit_mode) {
            case EditMode::None:
                break;
            case EditMode::Tile:
                if (CheckCollisionPointRec(GetMousePosition(), {position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing})) {
                    DrawRectangle(position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing, RED);
                    if (_level.tile_map.get_at(position.x, position.y) != entt::null) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            _core->registry.ctx().erase<EntitySelected>();
                            _core->registry.ctx().emplace<editor::EntitySelected>(_level.tile_map.get_at(position.x, position.y));
                        }
                    }

                }
                break;
            case EditMode::Wall:
                if (CheckCollisionPointRec(GetMousePosition(), {position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing})) {
                    DrawRectangle(position.x * _spacing + _offset.x, position.y * _spacing + _offset.y, _spacing, _spacing, BLUE);
                }
                break;
            default:
                break;
        }
    }
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
void MapView::update() {

}
void MapView::save_level(const SaveLevel& level) {
    try {
        _level.save("../../assets/levels/" + level.path);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    _core->dispatcher.enqueue<RefreshLevels>();
}
