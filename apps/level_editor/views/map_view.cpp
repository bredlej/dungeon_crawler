//
// Created by Patryk Szczypień on 05/04/2023.
//
#include <map_view.hpp>

static inline bool is_tile_select(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Tile && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Select;
}
static inline bool is_tile_paint(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Tile && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Paint;
}
static inline bool is_tile_fill(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Tile && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Fill;
}
static inline bool is_wall_select(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Wall && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Select;
}
static inline bool is_wall_paint(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Wall && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Paint;
}
static inline bool is_wall_line(const Core &core) {
    return core.registry.ctx().find<CurrentEditMode>()->edit_mode == EditMode::Wall && core.registry.ctx().find<CurrentEditModeTool>()->edit_mode_tool == EditModeTool::Line;
}

/**
 * @brief Renders and updates the MapView.
 *
 * This function is responsible for rendering and updating the MapView. It performs the following tasks:
 * - Clears the background to black.
 * - Draws the grid.
 * - Draws the tile map.
 * - Draws the wall map.
 * - Handles user interactions with the grid.
 * - Draws the mouseover tile on the grid.
 * - Selects tiles in a rectangle if tile select mode is active.
 * - Performs the paint action if tile paint mode is active.
 *
 * @note This function does not return any value.
 * @note This function is noexcept.
 *
 * @see MapView
 * @see ClearBackground
 * @see _draw_grid
 * @see _draw_tile_map
 * @see _draw_wall_map
 * @see _handle_user_interactions_with_grid
 * @see _draw_mouseover_tile_on_grid
 * @see is_tile_select
 * @see _select_tiles_in_rectangle
 * @see is_tile_paint
 */
void MapView::render() noexcept {
    using namespace editor;
    ClearBackground(BLACK);
    _draw_grid();
    _draw_tile_map();
    _draw_wall_map();
    _handle_user_interactions_with_grid();
    _draw_mouseover_tile_on_grid();
    if (is_tile_select(*_core)) {
        _select_tiles_in_rectangle();
    } else if (is_tile_paint(*_core)) {
    }
}

/**
 * @brief This function loads a level into the map view.
 *
 * It clears the selection, sets the edit mode to "None",
 * updates the level file name, clears the entities selection,
 * and loads the specified level.
 *
 * @param level The level to be loaded.
 */
void MapView::load_level(LoadLevel level) {
    _core->dispatcher.enqueue<ClearSelection>();
    _core->registry.ctx().erase<CurrentEditMode>();
    _core->registry.ctx().emplace<CurrentEditMode>(EditMode::None);
    _core->registry.ctx().erase<LevelFileName>();
    _core->registry.ctx().emplace<LevelFileName>(level.path.filename());
    _core->registry.ctx().erase<EntitiesSelected>();
    _level.load(level.path);
}

/**
 * @brief Update the MapView.
 *
 * This function is responsible for updating the MapView based on user input.
 *
 * If the left mouse button is down, it checks if the drag start position is zero. If it is, it sets the drag start position to the current mouse position. Otherwise, it sets the drag end position to the current mouse position.
 *
 * If the right mouse button is pressed and a tile is selected, it enqueues a ClearSelection event in the core dispatcher.
 *
 * If neither the left mouse button is down nor the right mouse button is pressed, it resets both the drag start and drag end positions to zero.
 */
void MapView::update() noexcept {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (_mouse_drag_start_position.x == 0 && _mouse_drag_start_position.y == 0) {
            _mouse_drag_start_position = GetMousePosition();
        } else {
            _mouse_drag_end_position = GetMousePosition();
        }
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && is_tile_select(*_core)) {
        _core->dispatcher.enqueue<ClearSelection>();
    } else {
        _mouse_drag_start_position = _mouse_drag_end_position = (Vector2){0, 0};
    }
}

/**
 * @brief Saves the given level.
 *
 * This function saves the level specified by the given `SaveLevel` object.
 * The level is saved to the specified path relative to the "assets/levels"
 * directory.
 *
 * If any exception occurs during the level saving process, the error message
 * will be printed to the standard error output.
 *
 * After the level is successfully saved, a `RefreshLevels` event is enqueued
 * to the dispatcher.
 *
 * @param level The `SaveLevel` object specifying the level to be saved.
 * @return None.
 *
 * @note This function assumes that `_core` member variable is a valid pointer
 *       to the core game engine instance and `_level` member variable is an
 *       instance of a class that provides a `save` function to save the level.
 *       The path of the level file is obtained by concatenating the specified
 *       path with the relative path "assets/levels" directory.
 */
void MapView::save_level(const SaveLevel &level) {
    try {
        _level.save("../../assets/levels/" + level.path);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    _core->dispatcher.enqueue<RefreshLevels>();
}

/**
 * @brief Handles user interactions with a tile on the grid.
 *
 * Checks if the mouse cursor is over the specified tile's area and performs actions accordingly.
 * If the mouse left button is pressed, it updates the selected entity and places a tile if in tile paint mode.
 *
 * @param x The x-coordinate of the tile.
 * @param y The y-coordinate of the tile.
 */
void MapView::_handle_user_interactions_with_tile_on_grid(const int32_t x, const int32_t y) const {
    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size})) {
        DrawRectangleLines(x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size, GREEN);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            _core->registry.ctx().erase<EntitySelected>();
            _core->registry.ctx().emplace<editor::EntitySelected>(_level.tile_map.get_at(x, y));
            if (is_tile_paint(*_core)) {
                _core->dispatcher.enqueue<TilePlacement>(x, y);
            }
        }
    }
}

/**
 * \brief Draws a mouseover line on the grid.
 *
 * \param x The x-coordinate of the grid cell.
 * \param y The y-coordinate of the grid cell.
 * \param is_horizontal_line Determines whether to draw a horizontal line or a vertical line.
 *
 * The _draw_mouseover_line_on_grid function is used to draw a line on the grid to indicate a mouseover effect.
 * It takes the x and y coordinates of the grid cell as input and a boolean value to determine whether to draw a horizontal or vertical line.
 * The line is drawn using the DrawLine function provided by the underlying graphics library with a red color.
 * The position of the line is calculated based on the cell size, offset, and whether it's a horizontal or vertical line.
 *
 * \remark This function assumes that the graphics library and necessary dependencies have been properly initialized.
 */
void MapView::_draw_mouseover_line_on_grid(int32_t x, int32_t y, bool is_horizontal_line) const {
    if (is_horizontal_line) {
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y - 1, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y - 1, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y + 1, x * _cell_size + _offset.x + _cell_size, y * _cell_size + _offset.y + 1, RED);
    } else {
        DrawLine(x * _cell_size + _offset.x - 1, y * _cell_size + _offset.y, x * _cell_size + _offset.x - 1, y * _cell_size + _offset.y + _cell_size, RED);
        DrawLine(x * _cell_size + _offset.x, y * _cell_size + _offset.y, x * _cell_size + _offset.x, y * _cell_size + _offset.y + _cell_size, RED);
        DrawLine(x * _cell_size + _offset.x + 1, y * _cell_size + _offset.y, x * _cell_size + _offset.x + 1, y * _cell_size + _offset.y + _cell_size, RED);
    }
}

/**
 * @brief Perform a tool action on a wall in the map view.
 *
 * This function is used to perform a specific action on a wall in the map view using the specified edit mode tool.
 *
 * @param editModeTool The edit mode tool to use for the action.
 * @param x The x-coordinate of the wall.
 * @param y The y-coordinate of the wall.
 * @param isUpperWall Indicates whether the wall is an upper wall or not.
 */
void MapView::_perform_tool_action_on_wall(const EditModeTool editModeTool, int32_t x, int32_t y, bool isUpperWall) const {
    auto xMod = isUpperWall ? 0 : 1;
    auto yMod = isUpperWall ? 1 : 0;
    std::vector<std::pair<MapPosition, MapPosition>> walls;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (editModeTool) {
            case EditModeTool::Select:
                walls.emplace_back(MapPosition{x - xMod, y - yMod}, MapPosition{x, y});
                _core->dispatcher.enqueue<WallSelected>(walls);
                break;
            case EditModeTool::Paint:
                _core->dispatcher.enqueue<WallAdded>(WallType::RUINS_01, MapPosition{x - xMod, y - yMod}, MapPosition{x, y});
                break;
            default:
                break;
        }
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        switch (editModeTool) {
            case EditModeTool::Select:
                _core->registry.ctx().erase<WallSelected>();
                break;
            case EditModeTool::Paint:
                _core->dispatcher.enqueue<WallRemoved>(MapPosition{x - xMod, y - yMod}, MapPosition{x, y});
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Handles user interactions with a wall on the grid.
 *
 * This function is responsible for handling user interactions with a wall on the grid.
 * It checks if the mouse cursor is within the bounds of the specified wall cell and performs
 * the appropriate actions based on the current edit mode tool.
 *
 * @param x The x-coordinate of the wall cell.
 * @param y The y-coordinate of the wall cell.
 *
 * @note This function assumes that the `_core` member variable is valid and initialized, and that
 *       the `CheckCollisionPointRec` and `_perform_tool_action_on_wall` functions are defined and implemented.
 */
void MapView::_handle_user_interactions_with_wall_on_grid(int32_t x, int32_t y) const {
    static constexpr float divider = 5.0f;
    const auto *edit_mode_tool = _core->registry.ctx().find<CurrentEditModeTool>();

    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x, y * _cell_size + _offset.y - (_cell_size / divider), _cell_size, 2 * (_cell_size / divider)})) {
        // upper
        _draw_mouseover_line_on_grid(x, y, true);
        _perform_tool_action_on_wall(edit_mode_tool->edit_mode_tool, x, y, true);
    }

    if (CheckCollisionPointRec(GetMousePosition(), {x * _cell_size + _offset.x - (_cell_size / divider), y * _cell_size + _offset.y, 2 * (_cell_size / divider), _cell_size})) {
        // left
        _draw_mouseover_line_on_grid(x, y, false);
        _perform_tool_action_on_wall(edit_mode_tool->edit_mode_tool, x, y, false);
    }
}

/**
 * @brief Sets a new level in the MapView.
 *
 * This function clears the existing level and creates a new level with
 * the given dimensions.
 *
 * @param new_level The information about the new level.
 *
 * @see MapLevel::clear()
 * @see MapLevel::new_level()
 */
void MapView::new_level(NewLevel new_level) {
    _level.clear();
    _level.new_level(new_level.width, new_level.height);
}

/**
 * @brief Draws a grid on the map view.
 *
 * This function draws a grid on the map view using the current level's tile map.
 * Each tile in the tile map represents a cell in the grid. The cells are drawn as rectangles
 * with gray lines.
 *
 * @note This function assumes that the `MapView` object has a valid level and tile map.
 * If not, the behavior is undefined.
 */
void MapView::_draw_grid() const {
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            DrawRectangleLines(x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size, palette::gray);
        }
    }
}

/**
 * @brief Draws a rectangle around the mouseover tile on the grid.
 *
 * This function is responsible for drawing a rectangle around the tile that is currently being hovered by the mouse, on the game grid.
 * If there is a tile currently being hovered, the rectangle will be drawn using the given cell size and offset values. The color of the rectangle is set to yellow.
 *
 * @details The function iterates through all the tiles on the grid, checking if the mouse is currently hovering over a particular tile using the position_hovered variable.
 * If the mouse is indeed hovering over a tile, a rectangle is drawn using the DrawRectangleLinesEx function from the palette library.
 * The position of the rectangle is set based on the current x and y coordinates of the tile, multiplied by the cell size and offset values.
 * The size of the rectangle is set to the cell size.
 * The thickness of the rectangle lines is set to 3 pixels.
 * Finally, the color of the rectangle is set to yellow.
 *
 * @note This function assumes the existence of the `palette` namespace and the `DrawRectangleLinesEx` function.
 *
 * @see MapView::_level
 * @see MapView::_core
 * @see MapView::_cell_size
 * @see MapView::_offset
 * @see MapPositionHovered
 *
 * @sa palette::yellow
 * @sa DrawRectangleLinesEx
 */
void MapView::_draw_mouseover_tile_on_grid() const {
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            if (auto *position_hovered = _core->registry.ctx().find<MapPositionHovered>()) {
                if (position_hovered->position.x == x && position_hovered->position.y == y) {
                    DrawRectangleLinesEx({x * _cell_size + _offset.x, y * _cell_size + _offset.y, _cell_size, _cell_size}, 3, palette::yellow);
                }
            }
        }
    }
}

/**
 * @brief Draw the tile map on the screen.
 *
 * This function iterates over each tile in the tile_map and renders it on the screen.
 * If the entity associated with a tile is null, a gray rectangle is drawn at the tile's position.
 * If the entity is not null, the appropriate texture is drawn based on the type of the entity (Floor).
 * If the entity is not a Floor, a dark gray rectangle is drawn at the tile's position.
 *
 * @note This function assumes that the _core and _level.tile_map variables are properly initialized.
 *
 * @note This function also draws a red rectangle outlining the currently selected area, if one exists.
 */
void MapView::_draw_tile_map() const {
    for (const auto &tile: _level.tile_map._tiles) {
        components::tiles::MapPosition position = _core->registry.get<components::tiles::MapPosition>(tile.entity);
        if (tile.entity == entt::null) {
            DrawRectangle(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, GRAY);
        } else {
            if (_core->registry.any_of<components::tiles::Floor>(tile.entity)) {
                components::tiles::Floor floor = _core->registry.get<components::tiles::Floor>(tile.entity);
                DrawTextureRec(_core->registry.ctx().find<EditorAssets>()->_textures[floor.type].get(), {0, 0, 25, 25}, {position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y}, WHITE);
                DrawRectangleLines(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, palette::dark_gray);
            } else {
                DrawRectangle(position.x * _cell_size + _offset.x, position.y * _cell_size + _offset.y, _cell_size, _cell_size, palette::dark_gray);
            }
        }
    }
    if (auto *rectangle_selected = _core->registry.ctx().find<RectangleSelected>()) {
        DrawRectangleLines(rectangle_selected->selection.x, rectangle_selected->selection.y, rectangle_selected->selection.width, rectangle_selected->selection.height, RED);
    }
}


/**
 * @brief Draw the wall map.
 *
 * This function is responsible for drawing the wall map on the screen.
 * It iterates over the walls in the level's wall map and calls helper functions
 * to draw each wall. Additionally, if there is a wall selected, it also draws
 * the selected walls on top of the wall map.
 */
void MapView::_draw_wall_map() const {
    const auto *wall_selected = _core->registry.ctx().find<WallSelected>();
    static constexpr int wall_thickness = 3;
    static constexpr int door_thickness = 5;
    static constexpr Color wall_color = palette::light_brown;
    static constexpr Color door_color_open = palette::green;
    static constexpr Color door_color_closed = palette::red;

    for (const auto &wall: _level.wall_map._walls) {
        auto wall_data = _core->registry.get<components::tiles::Wall>(wall.entity);
        auto fields = std::make_pair(wall_data.field1, wall_data.field2);
        draw_wall_or_door(wall, fields, door_thickness, wall_thickness, wall_color, door_color_open, door_color_closed);
    }

    if (wall_selected) {
        for (const auto& pair: wall_selected->positions) {
            draw_selected_wall(pair, palette::yellow, wall_thickness);
        }
    }
}

/**
 * @brief Draws a wall or a door on the map view.
 *
 * This function takes a wall entity and a pair of map positions representing the fields where the wall or door should be drawn.
 * It also takes the door thickness, wall thickness, wall color, open door color, and closed door color as parameters.
 * If the given wall entity is a door, it calls the draw_door() function to draw the door with the specified colors based on its state.
 * Otherwise, it calls the draw_wall() function to draw the wall with the specified thickness and color.
 *
 * @param wall                The wall entity.
 * @param fields              The pair of map positions representing the fields where the wall or door should be drawn.
 * @param door_thickness      The thickness of the door.
 * @param wall_thickness      The thickness of the wall.
 * @param wall_color          The color of the wall.
 * @param door_color_open     The color of the door when it is open.
 * @param door_color_closed   The color of the door when it is closed.
 */
void MapView::draw_wall_or_door(const WallEntity& wall, const std::pair<MapPosition, MapPosition>& fields, int door_thickness,
                                int wall_thickness, Color wall_color, Color door_color_open, Color door_color_closed) const {
    const auto *door = _core->registry.try_get<Door>(wall.entity);

    if (door) {
        draw_door(fields, door_thickness, door->state == DoorStateType::CLOSED ? door_color_closed : door_color_open);
    } else {
        draw_wall(fields, wall_thickness, wall_color);
    }
}

/**
 * @brief Draw a door on the map view.
 *
 * This function draws a door between two map positions on the map view. The door can be either horizontal or vertical
 * based on the given positions. The door is drawn with the given thickness and color.
 *
 * @param fields The pair of map positions indicating the start and end positions of the door.
 * @param door_thickness The thickness of the door.
 * @param color The color of the door.
 *
 * @note The positions in the `fields` pair should be valid map positions within the map view.
 */
void MapView::draw_door(const std::pair<MapPosition, MapPosition>& fields, int door_thickness, const Color& color) const {
    const auto& [field1, field2] = fields;
    const bool is_horizontal = (field1.x < field2.x && field1.y == field2.y) || (field1.x > field2.x && field1.y == field2.y);
    const Rectangle draw_params = is_horizontal ? Rectangle{(field2.x * _cell_size + _offset.x) - 2, field2.y * _cell_size + _offset.y, static_cast<float>(door_thickness), _cell_size}
                                     : Rectangle{field2.x * _cell_size + _offset.x, (field2.y * _cell_size + _offset.y) - 2, _cell_size, static_cast<float>(door_thickness)};

    DrawRectangleLinesEx(draw_params, door_thickness, color);
}

/**
 * @brief Draw a wall on the map view.
 *
 * This function draws a wall on the map view between two given map positions. The thickness of the wall and the color can be customized.
 *
 * @param fields A pair of map positions representing the two ends of the wall.
 * @param wall_thickness The thickness of the wall in pixels.
 * @param color The color of the wall.
 *
 * @note The positions are given as (x, y) coordinates on the map, where (0, 0) is the top-left corner.
 * @note The start position of the wall is at the first field in the pair, and the end position is at the second field.
 * @note The wall is drawn as a straight line from start position to end position.
 * @note If the y-coordinates of both fields are the same, the line will be drawn horizontally.
 * @note If the y-coordinates are different, the line will be drawn vertically.
 * @note The line will be drawn within the specified wall thickness.
 * @note The color of the line can be customized using the provided color argument.
 */
void MapView::draw_wall(const std::pair<MapPosition, MapPosition>& fields, int wall_thickness, const Color& color) const {
    const auto& [field1, field2] = fields;
    if (field1.x < field2.x && field1.y == field2.y) {
        DrawLineEx({field2.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y}, {field2.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y + _cell_size}, wall_thickness, color);
    } else if (field1.x > field2.x && field1.y == field2.y) {
        DrawLineEx({field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y}, {field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y + _cell_size}, wall_thickness, color);
    } else if (field1.x == field2.x && field1.y < field2.y) {
        DrawLineEx({field1.x * _cell_size + _offset.x, field2.y * _cell_size + _offset.y}, {field1.x * _cell_size + _offset.x + _cell_size, field2.y * _cell_size + _offset.y}, wall_thickness, color);
    } else if (field1.x == field2.x && field1.y > field2.y) {
        DrawLineEx({field1.x * _cell_size + _offset.x, field1.y * _cell_size + _offset.y}, {field1.x * _cell_size + _offset.x + _cell_size, field1.y * _cell_size + _offset.y}, wall_thickness, color);
    }
}

/**
 * @brief Draws a selected wall on the map view.
 *
 * This function draws a selected wall between two positions on the map view.
 * The wall is drawn using the given color and thickness.
 * The positions are specified as a pair of MapPositions.
 *
 * @param pair The pair of MapPositions representing the start and end positions of the wall.
 * @param color The color of the wall.
 * @param wall_thickness The thickness of the wall.
 * @return void
 */
void MapView::draw_selected_wall(const std::pair<MapPosition, MapPosition>& pair, const Color& color, int wall_thickness) const {
    MapPosition position1 = pair.first;
    MapPosition position2 = pair.second;
    if (position1.x < position2.x && position1.y == position2.y) {
        DrawLineEx({position2.x * _cell_size + _offset.x, position1.y * _cell_size + _offset.y}, {position2.x * _cell_size + _offset.x, position2.y * _cell_size + _offset.y + _cell_size}, 3, palette::yellow);
    } else if (position1.x == position2.x && position1.y < position2.y) {
        DrawLineEx({position1.x * _cell_size + _offset.x, position2.y * _cell_size + _offset.y}, {position2.x * _cell_size + _offset.x + _cell_size, position2.y * _cell_size + _offset.y}, 3, palette::yellow);
    }
}

/**
 * @brief Handles user interactions with the grid.
 *
 * This function iterates through each cell in the grid and handles the user interactions based
 * on the current edit mode of the MapView. The edit mode can be either None, Tile, or Wall.
 * For each cell, the appropriate interaction function is called based on the edit mode.
 *
 * @note This function assumes that the MapView has a valid level and tile map.
 *
 * @see MapView::_handle_user_interactions_with_tile_on_grid()
 * @see MapView::_handle_user_interactions_with_wall_on_grid()
 */
void MapView::_handle_user_interactions_with_grid() const {
    for (uint32_t y = 0; y < _level.tile_map._height; y++) {
        for (uint32_t x = 0; x < _level.tile_map._width; x++) {
            switch (_core->registry.ctx().find<CurrentEditMode>()->edit_mode) {
                case EditMode::None:
                    break;
                case EditMode::Tile:
                    _handle_user_interactions_with_tile_on_grid(x, y);
                    break;
                case EditMode::Wall:
                    _handle_user_interactions_with_wall_on_grid(x, y);
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * @brief Selects the tiles within a rectangle defined by the start and end positions of a mouse drag.
 *
 * This function iterates over all tiles in the map and checks if their positions fall within the rectangle
 * defined by the start and end positions of a mouse drag. If a tile is within the rectangle, its position
 * is added to the selected_positions vector.
 *
 * Once all tiles have been checked, the selected_positions vector is used to enqueue a MapPositionSelected event,
 * erase any existing MapPositionSelected events, and add a new MapPositionSelected event to the entity registry.
 *
 * Additionally, a RectangleSelected event is created and added to the entity registry, representing the boundaries
 * of the selection rectangle. The rectangle is then drawn on the screen using DrawRectangleLinesEx function.
 *
 * @note The start and end positions of the mouse drag must be set before calling this function.
 * @note The level.tile_map must be initialized before calling this function.
 */
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
        std::vector<components::tiles::MapPosition> selected_positions;
        for (int32_t i = min_y; i <= max_y; i++) {
            for (int32_t j = min_x; j <= max_x; j++) {
                selected_positions.emplace_back(components::tiles::MapPosition{j, i});
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

/**
 * Places an empty tile at the specified position on the map.
 *
 * @param tile_placement The position of the tile to be placed.
 */
void MapView::_place_empty_tile(TilePlacement tile_placement) {
    if (auto entity = _core->registry.ctx().find<EntitySelected>()) {
        auto field = _level.tile_map.get_at(tile_placement.x, tile_placement.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::tiles::TileId>(field);
            _core->registry.emplace<components::tiles::MapPosition>(field, tile_placement.x, tile_placement.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
    }
}

/**
 * \brief Handle the selection of entities on the map.
 *
 * This function erases any existing EntitiesSelected component from the registry
 * and replaces it with a new EntitiesSelected component containing the selected entities.
 *
 * \param event The MapPositionSelected event containing the selected positions on the map.
 */
void MapView::_handle_entities_selection(const MapPositionSelected &event) {
    _core->registry.ctx().erase<EntitiesSelected>();
    std::vector<std::pair<components::tiles::MapPosition, entt::entity>> entities;
    for (const components::tiles::MapPosition &position: event.positions) {
        const auto entity = _level.tile_map.get_at(position.x, position.y);
        entities.emplace_back(position, entity);
    }
    _core->registry.ctx().emplace<EntitiesSelected>(entities);
}

/**
 * @brief Clears the rectangle selection from the map view.
 *
 * This function removes the RectangleSelected and EntitiesSelected components from the context registry
 * associated with the map view, effectively clearing the current rectangle selection.
 *
 * @param clearSelection The ClearSelection object used to specify the type of clear operation to perform.
 *                       This parameter is currently unused in this function.
 */
void MapView::_clear_rectangle_selection(ClearSelection clearSelection) {
    _core->registry.ctx().erase<RectangleSelected>();
    _core->registry.ctx().erase<EntitiesSelected>();
}

/**
 * \brief Place floor tiles on the map.
 *
 * This function is called when an event occurs to place floor tiles on the map.
 * It iterates over the given positions and checks if there is a floor tile at each position.
 * If no floor tile is found, a new tile is created and added to the tile map.
 * The floor component is then added or replaced on the tile.
 *
 * \param event The event containing the positions and floor tile component.
 */
void MapView::_place_floor(editor::PlaceTileComponent<Floor> event) {
    for (const auto &position: *event.positions) {
        auto field = _level.tile_map.get_at(position.x, position.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::tiles::TileId>(field);
            _core->registry.emplace<components::tiles::MapPosition>(field, position.x, position.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
        _core->registry.emplace_or_replace<components::tiles::Floor>(field, event.component.type);
    }
}

/**
 * @brief Places encounter chance on the map at specified positions.
 *
 * This function is called to place encounter chance on the map at the given positions.
 * It creates a new tile if the tile at the specified position does not exist and assigns
 * the encounter chance to the tile.
 *
 * @param event The event object containing the positions and the encounter chance to be placed.
 */
void MapView::_place_encounter_chance(editor::PlaceTileComponent<EncounterChance> event) {
    for (const auto &position: *event.positions) {
        auto field = _level.tile_map.get_at(position.x, position.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::tiles::TileId>(field);
            _core->registry.emplace<components::tiles::MapPosition>(field, position.x, position.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
        _core->registry.emplace_or_replace<components::values::EncounterChance>(field, event.component.chance);
    }
}

/**
 * \brief Updates the walkability of place tiles in the map view.
 *
 * This function is invoked when a PlaceTileComponent event is received. It iterates over the positions provided
 * in the event and updates the corresponding place tiles in the map view with the specified walkability value.
 * If a place tile does not exist at a position, a new one is created and added to the tile map with the updated walkability.
 *
 * \param event The PlaceTileComponent event containing the positions and walkability value.
 */
void MapView::_place_walkability(editor::PlaceTileComponent<Walkability> event) {
    for (const auto &position: *event.positions) {
        auto field = _level.tile_map.get_at(position.x, position.y);
        if (field == entt::null) {
            field = _core->registry.create();
            _core->registry.emplace<components::tiles::TileId>(field);
            _core->registry.emplace<components::tiles::MapPosition>(field, position.x, position.y);
            _level.tile_map._tiles.emplace_back(Tile{field});
        }
        _core->registry.emplace_or_replace<components::tiles::Walkability>(field, event.component.walkable);
    }
}

/**
 * @brief Places a wall in the map view.
 *
 * This function is called when a PlaceWallComponent event is received.
 * It places a wall at the specified positions in the map.
 *
 * @param event The event containing the wall positions to place.
 */
void MapView::_place_wall(editor::PlaceWallComponent<Wall> event) {
    for (const auto &wall_selected: *event.positions) {
        auto wall = _level.wall_map.get_between(wall_selected.first, wall_selected.second);
        if (wall == entt::null) {
            wall = _core->registry.create();
            _level.wall_map._walls.emplace_back(WallEntity{wall});
        }
        _core->registry.emplace_or_replace<components::tiles::Wall>(wall, event.component.type, wall_selected.first, wall_selected.second);
    }
}

/**
 * \brief Places a door on the map.
 *
 * This function is called when a PlaceWallComponent event with a Door component is received. It places a door on the specified wall positions.
 *
 * \param event The PlaceWallComponent event with a Door component.
 *
 * \see editor::PlaceWallComponent
 */
void MapView::_place_door(editor::PlaceWallComponent<Door> event) {
    for (const auto &wall_selected: *event.positions) {
        auto wall = _level.wall_map.get_between(wall_selected.first, wall_selected.second);
        if (wall == entt::null) {
            wall = _core->registry.create();
            _level.wall_map._walls.emplace_back(WallEntity{wall});
            _core->registry.emplace<components::tiles::Wall>(wall, WallType::RUINS_01, wall_selected.first, wall_selected.second);
        }
        _core->registry.emplace_or_replace<components::tiles::Door>(wall, event.component.type_closed, event.component.type_opened, event.component.state);
    }
}

/**
 * @brief Erases the selected tiles from the tile map and registry.
 *
 * This function removes the selected tiles from the tile map and the entity registry.
 * It also removes the registry component for storing the selected entities.
 * If there are any selected positions, it enqueues an event to indicate the updated selection.
 */
void MapView::_erase_selected_tiles() {
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
                                 }),
                                 _level.tile_map._tiles.end());
    _core->registry.ctx().erase<EntitiesSelected>();

    auto *selected_positions = _core->registry.ctx().find<MapPositionSelected>();
    if (selected_positions) {
        _core->dispatcher.enqueue<MapPositionSelected>(selected_positions->positions);
    }
}

/**
 * \fn void MapView::_select_wall(const WallSelected &wallSelected)
 * \brief Selects a wall in the MapView.
 *
 * This function selects a wall in the MapView by updating the WallSelected component in the registry.
 * It removes any existing WallSelected components from the registry and adds the provided wallSelected component.
 *
 * \param wallSelected The WallSelected component to be added to the registry.
 *
 * \return void
 */
void MapView::_select_wall(const WallSelected &wallSelected) {
    _core->registry.ctx().erase<WallSelected>();
    _core->registry.ctx().emplace<WallSelected>(wallSelected);
}

/**
 * @brief Add/update a wall on the map.
 *
 * This function adds or updates a wall on the map based on the given WallAdded object.
 * If a wall with the same positions already exists, it will be updated with the new type.
 * Otherwise, a new wall will be created and added to the wall map.
 *
 * @param wallAdded The WallAdded object containing the wall's type and positions.
 */
void MapView::_add_wall(const WallAdded &wallAdded) {
    bool has_wall = false;
    _core->registry.view<Wall>().each([this, &wallAdded, &has_wall](const entt::entity &entity, const Wall &wall) {
        if (wall.field1.x == wallAdded.position1.x && wall.field1.y == wallAdded.position1.y && wall.field2.x == wallAdded.position2.x && wall.field2.y == wallAdded.position2.y) {
            has_wall = true;
            _core->registry.emplace_or_replace<components::tiles::Wall>(entity, wallAdded.type, wallAdded.position1, wallAdded.position2);
        }
    });
    if (!has_wall) {
        auto wall = _core->registry.create();
        _core->registry.emplace<components::tiles::Wall>(wall, wallAdded.type, wallAdded.position1, wallAdded.position2);
        _level.wall_map._walls.emplace_back(WallEntity{wall});
    }
}

/**
 * @brief Remove a wall from the map.
 *
 * This function removes a wall from the map based on the provided wallRemoved object.
 *
 * @param wallRemoved The information about the wall to be removed.
 */
void MapView::_remove_wall(const WallRemoved &wallRemoved) {
    entt::entity to_remove = entt::null;
    _core->registry.view<Wall>().each([this, &wallRemoved, &to_remove](const entt::entity &entity, const Wall &wall) {
        if (wall.field1.x == wallRemoved.position1.x && wall.field1.y == wallRemoved.position1.y && wall.field2.x == wallRemoved.position2.x && wall.field2.y == wallRemoved.position2.y || wall.field1.x == wallRemoved.position2.x && wall.field1.y == wallRemoved.position2.y && wall.field2.x == wallRemoved.position1.x && wall.field2.y == wallRemoved.position1.y) {
            to_remove = entity;
        }
    });
    if (to_remove != entt::null) {
        _level.wall_map._walls.erase(std::remove_if(_level.wall_map._walls.begin(), _level.wall_map._walls.end(), [to_remove](const WallEntity &wall) {
                                         return wall.entity == to_remove;
                                     }),
                                     _level.wall_map._walls.end());
        _core->registry.destroy(to_remove);
    }
}
