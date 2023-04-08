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

using namespace editor;

class MapView : public UIView {
public:
    explicit MapView(std::shared_ptr<Core> core, Rectangle dimension) noexcept : UIView{core}, _dimension(dimension), _level{core} {
        texture = LoadRenderTexture(static_cast<int>(dimension.width), static_cast<int>(dimension.height));
        core->dispatcher.sink<ChangeEditMode>().connect<&MapView::set_edit_mode>(this);
        core->dispatcher.sink<LoadLevel>().connect<&MapView::load_level>(this);
        core->dispatcher.sink<SaveLevel>().connect<&MapView::save_level>(this);
    }
    MapView(const MapView &) noexcept = delete;
    MapView(MapView &&) noexcept = delete;
    MapView &operator=(const MapView &) noexcept = delete;
    MapView &operator=(MapView &&) noexcept = delete;
    void render() override;
    void update() override;
    ~MapView() override {
        UnloadTexture(texture.texture);
    };
    RenderTexture2D texture{};
    void set_edit_mode(ChangeEditMode change_edit_mode);
    void load_level(LoadLevel level);
    void save_level(const SaveLevel& level);
private:
    Rectangle _dimension{};
    Vector2 _offset{15.0f, 35.0f};
    Vector2 _grid_size{20.0f, 20.0f};
    float _spacing{25.0f};
    EditMode _edit_mode{EditMode::Wall};
    Level _level;
};
#endif//DUNGEON_CRAWLER_MAP_VIEW_HPP