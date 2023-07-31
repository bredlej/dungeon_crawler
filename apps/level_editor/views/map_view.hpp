//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAP_VIEW_HPP
#define DUNGEON_CRAWLER_MAP_VIEW_HPP
#include "engine/core.hpp"
#include <components.hpp>
#include <editor_assets.hpp>
#include <events.hpp>
#include <filesystem>
#include <level.hpp>
#include <level_parser.hpp>
#include <memory>
#include <views/colors.hpp>
using namespace editor;
using namespace components::tiles;
using namespace components::values;

class MapView : public UIView<MapView> {
public:
    explicit MapView(std::shared_ptr<Core> core, Rectangle dimension) noexcept : UIView{core}, _dimension(dimension), _level{core} {
        texture = LoadRenderTexture(static_cast<int>(dimension.width), static_cast<int>(dimension.height));
        _core->dispatcher.sink<NewLevel>().connect<&MapView::new_level>(this);
        _core->dispatcher.sink<LoadLevel>().connect<&MapView::load_level>(this);
        _core->dispatcher.sink<SaveLevel>().connect<&MapView::save_level>(this);
        _core->dispatcher.sink<MapPositionSelected>().connect<&MapView::_handle_entities_selection>(this);
        _core->dispatcher.sink<ClearSelection>().connect<&MapView::_clear_rectangle_selection>(this);
        _core->dispatcher.sink<editor::PlaceTileComponent<Floor>>().connect<&MapView::_place_floor>(this);
        _core->dispatcher.sink<editor::PlaceTileComponent<EncounterChance>>().connect<&MapView::_place_encounter_chance>(this);
        _core->dispatcher.sink<editor::PlaceTileComponent<Walkability>>().connect<&MapView::_place_walkability>(this);
        _core->dispatcher.sink<editor::PlaceWallComponent<Wall>>().connect<&MapView::_place_wall>(this);
        _core->dispatcher.sink<editor::PlaceWallComponent<Door>>().connect<&MapView::_place_door>(this);
        _core->dispatcher.sink<editor::RemoveAllSelectedEntities>().connect<&MapView::_erase_selected_tiles>(this);
        _core->dispatcher.sink<editor::WallSelected>().connect<&MapView::_select_wall>(this);
        _core->dispatcher.sink<editor::WallAdded>().connect<&MapView::_add_wall>(this);
        _core->dispatcher.sink<editor::WallRemoved>().connect<&MapView::_remove_wall>(this);
        _core->dispatcher.sink<editor::TilePlacement>().connect<&MapView::_place_empty_tile>(this);
    }
    MapView(const MapView &) noexcept = delete;
    MapView(MapView &&) noexcept = delete;
    MapView &operator=(const MapView &) noexcept = delete;
    MapView &operator=(MapView &&) noexcept = delete;
    void render() noexcept override;
    void update() noexcept override;
    ~MapView() override {
        UnloadTexture(texture.texture);
    };
    RenderTexture2D texture{};
    void new_level(NewLevel new_level);
    void load_level(LoadLevel level);
    void save_level(const SaveLevel &level);

private:
    Rectangle _dimension{};
    Vector2 _offset{15.0f, 35.0f};
    Vector2 _grid_size{20.0f, 20.0f};
    Vector2 _mouse_drag_start_position{};
    Vector2 _mouse_drag_end_position{};
    float _cell_size{25.0f};
    EditMode _edit_mode{EditMode::Wall};
    Level _level;
    std::vector<components::tiles::MapPosition> _selected_positions;

    void _handle_entities_selection(const MapPositionSelected &mapPositionSelected);
    void _clear_rectangle_selection(ClearSelection clearSelection);
    void _draw_grid() const;
    void _draw_tile_map() const;
    void _draw_wall_map() const;
    void draw_wall_or_door(const WallEntity& wall, const std::pair<MapPosition, MapPosition>& fields, int door_thickness,
                               int wall_thickness, Color wall_color, Color door_color_open, Color door_color_closed) const;
    void draw_door(const std::pair<MapPosition, MapPosition>& fields, int door_thickness, const Color& color) const;
    void draw_wall(const std::pair<MapPosition, MapPosition>& fields, int wall_thickness, const Color& color) const;
    void draw_selected_wall(const std::pair<MapPosition, MapPosition>& pair, const Color& color, int wall_thickness) const;
    void _draw_mouseover_tile_on_grid() const;
    void _handle_user_interactions_with_grid() const;
    void _select_tiles_in_rectangle();
    void _place_empty_tile(TilePlacement tile_placement);
    void _handle_user_interactions_with_tile_on_grid(const int32_t x, const int32_t y) const;
    void _draw_mouseover_line_on_grid(int32_t x, int32_t y, bool is_horizontal_line) const;
    void _perform_tool_action_on_wall(const EditModeTool editModeTool, int32_t x, int32_t y, bool isUpperWall) const;
    void _handle_user_interactions_with_wall_on_grid(int32_t x, int32_t y) const;

    void _place_floor(editor::PlaceTileComponent<Floor> event);
    void _place_walkability(editor::PlaceTileComponent<Walkability> event);
    void _place_encounter_chance(editor::PlaceTileComponent<EncounterChance> event);

    void _place_wall(editor::PlaceWallComponent<Wall> event);
    void _place_door(editor::PlaceWallComponent<Door> event);

    void _erase_selected_tiles();

    void _select_wall(const WallSelected &wallSelected);
    void _add_wall(const WallAdded &wallAdded);
    void _remove_wall(const WallRemoved &wallRemoved);
};
#endif//DUNGEON_CRAWLER_MAP_VIEW_HPP
