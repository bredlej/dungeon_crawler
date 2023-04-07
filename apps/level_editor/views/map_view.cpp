//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <map_view.hpp>

void MapView::render() {
    ClearBackground(BLACK);

    //display a grid of 10x10 squares without filling them
    for (int x = 0; x < _grid_size.y; x++) {
        for (int y = 0; y < _grid_size.x; y++) {
            DrawRectangleLines(x * _spacing + _offset.x, y * _spacing+ _offset.y, _spacing, _spacing, GRAY);
            switch (_core->registry.ctx().find<CurrentEditMode>()->edit_mode) {
                case EditMode::Tile:
                    if (CheckCollisionPointRec(GetMousePosition(), {x * _spacing + _offset.x, y * _spacing + _offset.y, _spacing, _spacing})) {
                            DrawRectangle(x * _spacing + _offset.x, y * _spacing + _offset.y, _spacing, _spacing, RED);
                    }
                    break;
                case EditMode::Wall:
                    if (CheckCollisionPointRec(GetMousePosition(), {x * _spacing + _offset.x, y * _spacing + _offset.y, _spacing, _spacing})) {
                            DrawRectangle(x * _spacing + _offset.x, y * _spacing + _offset.y, _spacing, _spacing, BLUE);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    // Draw circles on the grid where lines intersect
    for (int i = 0; i < _grid_size.y + 1; i++) {
        for (int j = 0; j < _grid_size.x + 1; j++) {
            DrawCircleLines(i * _spacing + _offset.x, j * _spacing + _offset.y, 3.0f, RED);
        }
    }
}

void MapView::update() {

}