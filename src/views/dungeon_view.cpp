//
// Created by geoco on 19.11.2022.
//

#include <views/dungeon_view.hpp>

/**
 * \fn void DungeonView::_initialize() noexcept
 * \brief Initializes the DungeonView object.
 *
 * This function initializes the DungeonView object by setting the encounter chance to 0.00f.
 *
 * \note This function is noexcept.
 */
void DungeonView::_initialize() noexcept{
    _core->registry.ctx().emplace<components::values::EncounterChance>(0.00f);
}

/**
 * Draws a tile asset on the screen.
 *
 * @param assets A pointer to the assets object.
 * @param index The index of the asset to draw.
 * @param type The type of asset being drawn.
 * @param tint The tint color to apply to the asset.
 */
template <typename T>
static inline void draw_tile_asset(assets::Assets *assets, const size_t index, const T type, const Color tint) {
    DrawTexture(assets->_textures._tiles[static_cast<POVFloor>(index)][type].get(), 0, 0, tint);
}

/**
 * @brief Renders the point of view (POV) of the dungeon view.
 *
 * This function uses the render texture `_render_texture_pov` to render the POV of the dungeon view. It clears the background with the `BACKGROUND_COLOR` defined constant. It then calls several private rendering functions to render different elements of the POV, such as tiles, walls, and encounters, using the assets obtained from the core.
 *
 * @note This function assumes that the `_core` member variable of the DungeonView class is set and valid.
 * @note This function must be called within a rendering context.
 * @note This function does not return any value.
 *
 * @sa _render_tiles, _render_walls, _render_encounter
 */
void DungeonView::_render_pov() noexcept
{
    BeginTextureMode(_render_texture_pov);
    ClearBackground(BACKGROUND_COLOR);

    if (assets::Assets *assets = _core->get_assets())
    {
        _render_background(assets);
        _render_tiles(assets);
        _render_walls(assets);
        _render_encounter(assets);
    }

    EndTextureMode();
}

void DungeonView::_render_background(assets::Assets *assets) noexcept {
    auto player_view = _core->registry.view<components::general::Player, components::general::Direction>();
    player_view.each([&](const entt::entity entity, const components::general::Player player, components::general::Direction direction) {
        DrawTextureRec(assets->_textures._background[direction.direction].get(), Rectangle{0, 0, 320, 240}, Vector2{0, 0}, WHITE);
    });
}
/**
 * Renders the tiles of the dungeon view.
 *
 * This function iterates over the tiles in the player's field of view and renders them using the given assets. Each tile
 * can have a floor type and a fill color. If a tile entity is valid in the core registry, its floor type and fill color
 * will be used for rendering. Otherwise, the default floor type and fill color will be used.
 *
 * @param assets A pointer to the assets used for rendering.
 */
void DungeonView::_render_tiles(assets::Assets *assets) noexcept
{
    for (size_t i = 0; i < _player_fov_tile.field.size(); i++)
    {
        const auto tile_entity = _player_fov_tile.field[i];
        const Color default_color = _core->registry.valid(tile_entity) ? WHITE : RED;
        if (_core->registry.valid(tile_entity)){
            if(const auto floor = _core->registry.try_get<components::tiles::Floor>(_player_fov_tile.field[i]))
            {
                const auto tint = _core->registry.try_get<components::values::Tint>(_player_fov_tile.field[i]);
                draw_tile_asset(assets, i, floor->type, tint ? Color{tint->r, tint->g, tint->b, tint->a} : default_color);
            }

            if (const auto ceiling = _core->registry.try_get<components::tiles::Ceiling>(_player_fov_tile.field[i]))
            {
                const auto tint = _core->registry.try_get<components::values::Tint>(_player_fov_tile.field[i]);
                draw_tile_asset(assets, i, ceiling->type, tint ? Color{tint->r, tint->g, tint->b, tint->a} : default_color);
            }
        }
    }
}

/**
 * Renders the walls in the dungeon view.
 *
 * @param assets A pointer to the assets object containing wall textures.
 */
void DungeonView::_render_walls(assets::Assets *assets) noexcept
{
    for (const POVWall i: draw_order_walls)
    {
        const auto index = static_cast<const size_t>(i);
        if (_core->registry.valid(_player_fov_wall.field[index]))
        {
            const auto wall = _core->registry.try_get<components::tiles::Wall>(_player_fov_wall.field[index]);
            const auto tint = _core->registry.try_get<components::values::Tint>(_player_fov_wall.field[index]);
            const auto door = _core->registry.try_get<components::tiles::Door>(_player_fov_wall.field[index]);

            const Color fillColor = tint ? Color{tint->r, tint->g, tint->b, tint->a} : WHITE;
            if (door)
            {
                const auto& doorTexture = assets->_textures._tiles[i][door->state == DoorStateType::CLOSED ? door->type_closed : door->type_opened];
                DrawTexture(doorTexture.get(), 0, 0, fillColor);
            }
            else if (wall)
            {
                const auto& wallTexture = assets->_textures._tiles[i][wall->type];
                DrawTexture(wallTexture.get(), 0, 0, fillColor);
            }
        }
    }
}

/**
 * @brief Renders the encounter on the dungeon view.
 *
 * This function checks if there is an encounter in the current context of the dungeon view.
 * If an encounter exists, it draws the texture of a Goblin Warrior at the specified position.
 *
 * @param assets A pointer to the assets object that provides the required textures.
 */
void DungeonView::_render_encounter(assets::Assets *assets) noexcept
{
    if (_core->registry.ctx().contains<components::values::Encounter>())
    {
        DrawTexture(assets->_textures._beasts[Beast::GoblinWarrior].get(), 140, 55, WHITE);
    }
}

/**
 * @brief Draws the player frame on the minimap.
 *
 * This function takes a texture and a frame rectangle as input and draws the player frame on the minimap
 * based on the specified position and offset values.
 *
 * @param texture The texture to be used for drawing the player frame.
 * @param frame The rectangle defining the frame to be drawn from the texture.
 * @param position The position of the player on the minimap grid.
 * @param offset The offset to be applied to the player frame position on the minimap.
 * @return void
 */
static inline void minimap_draw_player_frame(const Texture2D &texture, const Rectangle &frame, components::tiles::MapPosition &position, ModXY offset) {
    DrawTextureRec(texture, frame, Vector2{static_cast<float>(position.x * MINIMAP_GRID_SIZE + offset.x), static_cast<float>(position.y * MINIMAP_GRID_SIZE + offset.y)}, WHITE);
}

/**
 * @brief Render the minimap on the render texture.
 *
 * This function renders the minimap on the render texture _render_texture_gui.
 * It draws the background image of the minimap, highlights the tiles that are in the field of view,
 * draws the player character on the minimap, and draws walls on the minimap.
 */
void DungeonView::_render_minimap() noexcept {

    BeginTextureMode(_render_texture_gui);
    ClearBackground(BACKGROUND_COLOR);
    DrawTexture(_core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Background].get(), 0, 0, Color{255,255,255,255});
    ModXY offset {10,10};
    for (auto tile: _level.tile_map._tiles) {
        if (_core->registry.valid(tile.entity)) {
            components::tiles::MapPosition position = _core->registry.get<components::tiles::MapPosition>(tile.entity);
            auto *tile_in_fov = _core->registry.try_get<components::tiles::InFovOfEntity>(tile.entity);
            if (tile_in_fov) {
                DrawRectangle(position.x * MINIMAP_GRID_SIZE + offset.x, position.y * MINIMAP_GRID_SIZE +offset.y, MINIMAP_GRID_SIZE, MINIMAP_GRID_SIZE, FOV_COLOR);
            }
            else {
                DrawRectangle(position.x * MINIMAP_GRID_SIZE + offset.x, position.y * MINIMAP_GRID_SIZE +offset.y, MINIMAP_GRID_SIZE, MINIMAP_GRID_SIZE, BACKGROUND_COLOR);
            }
        }
    }
    auto player_view = _core->registry.view<components::general::Player, components::general::Direction, components::tiles::MapPosition>();
    Texture2D player_texture = _core->get_assets()->_textures._gui[assets::dungeon_view::GUI::MiniMap::Player].get();
    player_view.each([&](const entt::entity entity, const components::general::Player player, components::general::Direction direction, components::tiles::MapPosition position) {
        switch (direction.direction) {
            case WorldDirection::NORTH: minimap_draw_player_frame(player_texture, {0,0,5,5}, position, offset); break;
            case WorldDirection::EAST: minimap_draw_player_frame(player_texture, {5,0,5,5}, position, offset); break;
            case WorldDirection::SOUTH: minimap_draw_player_frame(player_texture, {10,0,5,5}, position, offset); break;
            case WorldDirection::WEST: minimap_draw_player_frame(player_texture, {15,0,5,5}, position, offset); break;
        }
    });
    for (WallEntity wall: _level.wall_map._walls) {
        components::tiles::Wall wall_component = _core->registry.get<components::tiles::Wall>(wall.entity);
        components::tiles::MapPosition field1_position = wall_component.field1;
        components::tiles::MapPosition field2_position = wall_component.field2;
        if (field1_position.x == field2_position.x) {
            if (auto *door = _core->registry.try_get<components::tiles::Door>(wall.entity)){
                switch (door->state) {
                    case DoorStateType::OPEN: DrawLine(field1_position.x * MINIMAP_GRID_SIZE + offset.x, std::max(field1_position.y, field2_position.y) * MINIMAP_GRID_SIZE + offset.y, field1_position.x * MINIMAP_GRID_SIZE + offset.x + MINIMAP_GRID_SIZE, std::max(field1_position.y, field2_position.y) * 5 + offset.y, palette::green); break;
                    case DoorStateType::CLOSED: DrawLine(field1_position.x * MINIMAP_GRID_SIZE + offset.x, std::max(field1_position.y, field2_position.y) * MINIMAP_GRID_SIZE + offset.y, field1_position.x * MINIMAP_GRID_SIZE + offset.x + MINIMAP_GRID_SIZE, std::max(field1_position.y, field2_position.y) * 5 + offset.y, palette::red); break;
                }
            }
            else {
                DrawLine(field1_position.x * MINIMAP_GRID_SIZE + offset.x, std::max(field1_position.y, field2_position.y) * MINIMAP_GRID_SIZE + offset.y, field1_position.x * MINIMAP_GRID_SIZE + offset.x + MINIMAP_GRID_SIZE, std::max(field1_position.y, field2_position.y) * 5 + offset.y, WALL_COLOR);
            }
        }
        else if (field1_position.y == field2_position.y) {
            if (auto *door = _core->registry.try_get<components::tiles::Door>(wall.entity)){
                switch (door->state) {
                    case DoorStateType::OPEN: DrawLine(std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y, std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y + MINIMAP_GRID_SIZE, palette::green); break;
                    case DoorStateType::CLOSED: DrawLine(std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y, std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y + MINIMAP_GRID_SIZE, palette::red); break;
                }
            }
            else {
                DrawLine(std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y, std::max(field1_position.x, field2_position.x) * MINIMAP_GRID_SIZE + offset.x, field1_position.y * MINIMAP_GRID_SIZE + offset.y + MINIMAP_GRID_SIZE, WALL_COLOR);
            }
        }
    }
    EndTextureMode();
}

/**
 * \fn static inline void render_texture(const Texture &texture, const Rectangle &dimension)
 * \brief Renders a texture on the screen.
 *
 * This function renders a texture on the screen using the given dimension and color.
 *
 * \param texture The texture to render.
 * \param dimension The dimension of the rendered texture on the screen.
 *
 * \return None.
 */
static inline void render_texture(const Texture &texture, const Rectangle &dimension) {
    DrawTexturePro(texture,
                   Rectangle{0.0f, 0.0f, (float) texture.width, (float) -texture.height},
                   dimension,
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
}

/**
 * @brief Renders the dungeon view.
 *
 * This function is responsible for rendering the dungeon view by performing the following steps:
 *
 * 1. Begins drawing on the screen.
 * 2. Clears the background with the color BLACK.
 * 3. Renders the point-of-view (POV) of the dungeon.
 * 4. Renders the minimap of the dungeon.
 * 5. Defines the dimensions for the POV and GUI textures based on the window size.
 * 6. Renders the POV texture on the screen, using the appropriate dimensions based on whether the window is in fullscreen mode or not.
 * 7. Renders the GUI texture on the screen, using the appropriate dimensions based on whether the window is in fullscreen mode or not.
 * 8. Renders the user interface.
 * 9. Ends the drawing process.
 *
 * This function does not throw any exceptions.
 */
void DungeonView::render() noexcept {
    BeginDrawing();
    ClearBackground(BLACK);
    _render_pov();
    if (_core->registry.ctx().contains<components::values::ShowMinimap>()){
        _render_minimap();
    }
    static Rectangle POV_DIMENSION_FULLSCREEN = Rectangle{0,0, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    static Rectangle GUI_DIMENSION_FULLSCREEN = Rectangle{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.75f, static_cast<float>(GetMonitorHeight(GetCurrentMonitor())) * 0.1f, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.2f, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.2f};
    render_texture(_render_texture_pov.texture, IsWindowFullscreen() ? POV_DIMENSION_FULLSCREEN : POV_DIMENSION);
    render_texture(_render_texture_gui.texture, IsWindowFullscreen() ? GUI_DIMENSION_FULLSCREEN : GUI_DIMENSION);
    _ui.render();
    DrawFPS(20, 20);
    EndDrawing();
}

/**
 * @brief Updates the DungeonView.
 *
 * This function is called to update the DungeonView. It handles various user input and dispatches relevant events
 * to the core dispatcher. It also performs certain operations like calculating Field of View (FOV) and loading/saving
 * levels.
 *
 * @note This function is meant to be called continuously in a game loop to update the view.
 *
 * @note This function is noexcept, meaning it does not throw any exceptions.
 */
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
            _level.load("assets/Levels/Ruins/ruins0001.json");
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
    if (IsKeyPressed(KEY_M)) {
        if (_core->registry.ctx().contains<components::values::ShowMinimap>()) {
            std::printf("Erasing minimap\n");
            _core->registry.ctx().erase<components::values::ShowMinimap>();
        }
        else {
            _core->registry.ctx().emplace<components::values::ShowMinimap>();
        }
        std::printf("Is minimap: %d\n", _core->registry.ctx().contains<components::values::ShowMinimap>());
    }
    after_first_update = true;
}

/**
*
*/
template<size_t AMOUNT_WALLS_IN_FOV>
static void fill_player_fov_walls(std::array<entt::entity, AMOUNT_WALLS_IN_FOV> &player_fov_walls, const components::tiles::MapPosition player_position, const WallMap &wall_map, const TileMap &tile_map, const WorldDirection direction) {
    using namespace components::tiles;
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

/**
 * @brief Fills the player's field of view tiles based on their current position and direction.
 *
 * This function fills the given array `player_fov_tiles` with the tiles that are in the player's field of view.
 * The player's field of view is determined by their current position, direction, and the tile map.
 *
 * @tparam SIZE The size of the `player_fov_tiles` array.
 * @param player_fov_tiles Reference to the array to store the player's field of view tiles.
 * @param player_position The current position of the player.
 * @param tile_map The tile map containing the dungeon layout.
 * @param direction The current direction the player is facing.
 */
template <size_t SIZE>
static void fill_player_fov_tiles(std::array<entt::entity, SIZE> &player_fov_tiles, const components::tiles::MapPosition player_position, const TileMap &tile_map, const WorldDirection direction) {
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

/**
 *
 * @brief Set the tint on an entity.
 *
 * This function sets the tint for a given entity in the specified registry. The tint is represented by the
 * color channels (r, g, b, a), where r represents the red channel, g represents the green channel,
 * b represents the blue channel, and a represents the alpha channel.
 *
 * @param registry The registry in which the entity exists.
 * @param entity The entity for which to set the tint.
 * @param r The red channel value for the tint.
 * @param g The green channel value for the tint.
 * @param b The blue channel value for the tint.
 * @param a The alpha channel value for the tint.
 *
 * @return void
 *
 * @sa entt::registry, components::values::Tint
 *
 */
static inline void set_tint_on_entity(entt::registry &registry, const entt::entity entity, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (registry.valid(entity)) {
        registry.emplace_or_replace<components::values::Tint>(entity, r, g, b, a);
    }
}

template <size_t SIZE>
static inline void set_tiles_tint(entt::registry &registry, const std::array<entt::entity, SIZE> &player_fov_tiles) {
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F01], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F02], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F03], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F04], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F05], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F06], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F07], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F08], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F09], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F10], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F11], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F12], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F13], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F14], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F15], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_tiles[(size_t) assets::dungeon_view::POVFloor::F16], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
}

/**
 * @brief Sets the tint on a set of walls in the dungeon view.
 *
 * This function sets the tint on a set of walls in the dungeon view based on the player's field of view (FOV).
 * The function takes an entt::registry object and an array of entity IDs representing the walls in the player's FOV.
 *
 * @tparam SIZE The number of walls in the player's FOV.
 * @param registry The entt::registry object containing the wall entities.
 * @param player_fov_walls The array of entity IDs representing the walls in the player's FOV.
 *
 * @note This function assumes that `set_tint_on_entity` is a valid function that takes the following parameters in the order specified:
 *       - entt::registry& - The registry object.
 *       - entt::entity - The entity ID.
 *       - float - The red tint value.
 *       - float - The green tint value.
 *       - float - The blue tint value.
 *       - float - The alpha tint value.
 */
template <size_t SIZE>
static inline void set_walls_tint(entt::registry &registry, const std::array<entt::entity, SIZE> &player_fov_walls) {
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_E], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_N], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W01_S], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_E], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_N], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W02_S], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_N], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W03_S], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_N], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_S], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W04_W], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_N], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_S], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W05_W], TINT_DARKEST, TINT_DARKEST, TINT_DARKEST, NO_TINT);

    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W06_E], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_E], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W07_S], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W08_S], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_W], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W09_S], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W10_W], TINT_DARKER, TINT_DARKER, TINT_DARKER, NO_TINT);

    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_E], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W11_S], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W12_S], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_S], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W13_W], TINT_DARK, TINT_DARK, TINT_DARK, NO_TINT);

    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_E], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W14_S], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W15_S], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_S], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);
    set_tint_on_entity(registry, player_fov_walls[(size_t) assets::dungeon_view::POVWall::W16_W], TINT_A_BIT_DARK, TINT_A_BIT_DARK, TINT_A_BIT_DARK, NO_TINT);

}

/**
 * This function calculates the Field of View (FOV) for the player entity.
 * It clears the "InFovOfEntity" component from all entities in the registry.
 * It then retrieves the player entity's position and direction from the registry.
 * The FOV tiles and walls are filled based on the player's position, direction, tile map, and wall map.
 * The tint of the tiles and walls is set accordingly using the registry and FOV fields.
 * Finally, the "InFovOfEntity" component is added or replaced for each FOV tile.
 *
 * @note This function assumes that the player entity and relevant components exist in the registry.
 * @note The "tile_map" and "wall_map" fields must be set before calling this function.
 * @note The "field" fields of "_player_fov_tile" and "_player_fov_wall" must be initialized as empty containers.
 * @note The Registry class must have a clear() method that clears components by type.
 * @note The Registry class must have methods to retrieve and modify components from entities.
 * @note The set_tiles_tint() and set_walls_tint() functions must be defined and implemented.
 * @note The "valid()" and "emplace_or_replace()" methods must be available for the registry entities.
 *
 * @see components::tiles::InFovOfEntity
 * @see components::general::Player
 * @see components::general::Direction
 * @see components::tiles::MapPosition
 * @see components::values::Tint
 * @see fill_player_fov_tiles()
 * @see fill_player_fov_walls()
 * @see set_tiles_tint()
 * @see set_walls_tint()
 *
 * @return None
 */
void DungeonView::_calculate_fov() noexcept {
    _core->registry.clear<components::tiles::InFovOfEntity>();
    auto player_view = _core->registry.view<components::general::Player, components::general::Direction, components::tiles::MapPosition>();

    for (auto entity: player_view) {
        auto player_position = player_view.get<components::tiles::MapPosition>(entity);
        auto player_direction = player_view.get<components::general::Direction>(entity);

        fill_player_fov_tiles(_player_fov_tile.field, player_position, _level.tile_map, player_direction.direction);
        fill_player_fov_walls(_player_fov_wall.field, player_position, _level.wall_map, _level.tile_map, player_direction.direction);

        _core->registry.clear<components::values::Tint>();
        set_tiles_tint(_core->registry, _player_fov_tile.field);
        set_walls_tint(_core->registry, _player_fov_wall.field);

        for (entt::entity fov_tile: _player_fov_tile.field) {
            if (_core->registry.valid(fov_tile)) {
                _core->registry.emplace_or_replace<components::tiles::InFovOfEntity>(fov_tile, entity);
            }
        }
    }
}

/**
 * @brief Clears the dungeon state.
 *
 * This function clears the dungeon state by resetting the level, player FOV tile,
 * and player FOV wall fields to entt::null.
 *
 * @note This function is noexcept, meaning it does not throw any exceptions.
 *
 * @see DungeonView::_level, DungeonView::_player_fov_tile, DungeonView::_player_fov_wall
 */
void DungeonView::_clear() noexcept {
    _level.clear();
    _player_fov_tile.field.fill(entt::null);
    _player_fov_wall.field.fill(entt::null);
}
