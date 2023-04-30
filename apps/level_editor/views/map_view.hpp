//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_MAP_VIEW_HPP
#define DUNGEON_CRAWLER_MAP_VIEW_HPP
#include "engine/core.hpp"
#include <memory>
#include <events.hpp>
#include <level.hpp>
#include <level_parser.hpp>
#include <filesystem>
#include <editor_assets.hpp>
#include <views/colors.hpp>
#include <components.hpp>
using namespace editor;
using namespace components::fields;
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
        _core->dispatcher.sink<editor::PlaceComponent<Floor>>().connect<&MapView::_place_floor>(this);
        _core->dispatcher.sink<editor::PlaceComponent<EncounterChance>>().connect<&MapView::_place_encounter_chance>(this);
        _core->dispatcher.sink<editor::PlaceComponent<Walkability>>().connect<&MapView::_place_walkability>(this);
        _core->dispatcher.sink<editor::RemoveAllSelectedEntities>().connect<&MapView::_remove_all_selected_tiles>(this);
        _core->dispatcher.sink<editor::WallSelected>().connect<&MapView::_select_wall>(this);
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
    void save_level(const SaveLevel& level);
private:
    Rectangle _dimension{};
    Vector2 _offset{15.0f, 35.0f};
    Vector2 _grid_size{20.0f, 20.0f};
    Vector2 _mouse_drag_start_position{};
    Vector2 _mouse_drag_end_position{};
    float _cell_size{25.0f};
    EditMode _edit_mode{EditMode::Wall};
    Level _level;
    std::vector<components::fields::MapPosition> _selected_positions;

    void _handle_entities_selection(const MapPositionSelected &mapPositionSelected);
    void _clear_rectangle_selection(ClearSelection clearSelection);
    void _draw_grid() const;
    void _draw_tile_map() const;
    void _draw_wall_map() const;
    void _draw_selection_hovered_tile() const;
    void _draw_cursor() const;
    void _select_tiles_in_rectangle();
    void _check_tile_collision(int32_t x, int32_t y) const;
    void _check_wall_collision(int32_t x, int32_t y) const;

    void _place_floor(editor::PlaceComponent<Floor> event);
    void _place_walkability(editor::PlaceComponent<Walkability> event);
    void _place_encounter_chance(editor::PlaceComponent<EncounterChance> event);

    void _remove_all_selected_tiles();

    void _select_wall(const WallSelected &wallSelected);
};
#endif//DUNGEON_CRAWLER_MAP_VIEW_HPP
