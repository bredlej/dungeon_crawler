//
// Created by Patryk Szczypień on 14/04/2023.
//

#ifndef DUNGEON_CRAWLER_BLUEPRINT_HPP
#define DUNGEON_CRAWLER_BLUEPRINT_HPP
#include <component_renderer.hpp>
#include <core.hpp>
#include <cstdint>
#include <events.hpp>
#include <memory>
#include <typeindex>
#include <unordered_map>

enum class TileComponentType : uint8_t {
    None,
    Floor,
    EncounterChance,
    Walkability
};
static std::unordered_map<uint8_t, std::string> tile_component_type_to_string = {
        {static_cast<uint8_t>(TileComponentType::Floor), "Floor"},
        {static_cast<uint8_t>(TileComponentType::EncounterChance), "Encounter chance"},
        {static_cast<uint8_t>(TileComponentType::Walkability), "Walkability"}};

struct TileComponentSelected {
    TileComponentType type;
};
struct TileComponentAdded {
    TileComponentType type;
};
struct TileComponentRemoved {
    TileComponentType type;
};

template<typename... T>
struct Components {
    std::tuple<T...> components;
};

enum class WallComponentType : uint8_t {
    None,
    Wall,
    Door,
    SecretDoor
};
static std::unordered_map<uint8_t, std::string> wall_component_type_to_string = {
        {static_cast<uint8_t>(WallComponentType::Wall), "Wall"},
        {static_cast<uint8_t>(WallComponentType::Door), "Door"},
        {static_cast<uint8_t>(WallComponentType::SecretDoor), "Secret Door"}};

struct WallComponentSelected {
    WallComponentType type;
};

struct WallComponentAdded {
    WallComponentType type;
};

struct WallComponentRemoved {
    WallComponentType type;
};

using namespace components::tiles;
using namespace components::values;

class Blueprint;// forward declaration

class TileComponentActionHandler {
public:
    virtual void handleComponentSelection(Blueprint *blueprint) = 0;
    virtual void handleComponentPlacement(Blueprint *blueprint, editor::MapPositionSelected *) = 0;
    virtual ~TileComponentActionHandler() = default;
};

class WallComponentActionHandler {
public:
    virtual void handleComponentSelection(Blueprint *blueprint) = 0;
    virtual void handleComponentPlacement(Blueprint *blueprint, editor::WallSelected *) = 0;
    virtual ~WallComponentActionHandler() = default;
};

class Blueprint {
public:
    explicit Blueprint(const std::shared_ptr<Core> &core) : _core{core}, _component_renderer{core} {
        _core->dispatcher.sink<TileComponentSelected>().connect<&Blueprint::_tile_component_selected>(this);
        _core->dispatcher.sink<TileComponentAdded>().connect<&Blueprint::_tile_component_added>(this);
        _core->dispatcher.sink<TileComponentRemoved>().connect<&Blueprint::_tile_component_removed>(this);
        _core->dispatcher.sink<WallComponentSelected>().connect<&Blueprint::_wall_component_selected>(this);
        _core->dispatcher.sink<WallComponentAdded>().connect<&Blueprint::_wall_component_added>(this);
        _core->dispatcher.sink<WallComponentRemoved>().connect<&Blueprint::_wall_component_removed>(this);

        _initialize();
    };
    Blueprint(const Blueprint &) = delete;
    Blueprint(Blueprint &&) = default;
    Blueprint &operator=(const Blueprint &) = delete;
    Blueprint &operator=(Blueprint &&) = delete;
    ~Blueprint() = default;
    void render_and_update();


    template<typename T>
    void renderTileComponentSelection();
    template<typename T>
    void renderWallComponentSelection();
    template<typename T>
    void enqueueTileComponentPlacement(std::vector<components::tiles::MapPosition> *positions);
    template<typename T>
    void enqueueWallComponentPlacement(std::vector<std::pair<components::tiles::MapPosition, components::tiles::MapPosition>> *positions);

private:
    ComponentRenderer _component_renderer;
    std::shared_ptr<Core> _core;

    std::vector<TileComponentType> _added_tile_component_types;
    std::vector<uint8_t> _all_available_tile_component_types;
    uint8_t _selected_field_component_type{static_cast<uint8_t>(TileComponentType::None)};

    std::vector<WallComponentType> _added_wall_component_types;
    std::vector<uint8_t> _all_available_wall_component_types;
    uint8_t _selected_wall_component_type{static_cast<uint8_t>(WallComponentType::None)};

    std::unordered_map<TileComponentType, std::unique_ptr<TileComponentActionHandler>> _tile_component_handlers;
    std::unordered_map<WallComponentType, std::unique_ptr<WallComponentActionHandler>> _wall_component_handlers;

    void _initialize();
    void _initialize_tile_components();
    void _initialize_wall_components();

    void _tile_component_selected(TileComponentSelected type);
    void _tile_component_added(TileComponentAdded type);
    void _tile_component_removed(TileComponentRemoved type);
    void _wall_component_selected(WallComponentSelected type);

    void _wall_component_added(WallComponentAdded type);
    void _wall_component_removed(WallComponentRemoved type);

    template<typename COMPONENT_TYPE>
    void _render_component_properties(int index, auto &type, auto &type_to_string, auto &component_handlers);
    static void _render_available_component_combobox(const std::vector<uint8_t> &available_component_types,
                                                     const uint8_t &selected_component_type,
                                                     const std::unordered_map<uint8_t, std::string> &component_type_to_string,
                                                     const std::function<void(uint8_t)> &on_component_selected_callback,
                                                     const std::function<void(uint8_t)> &on_add_button_clicked_callback);

    Components<Floor, Walkability, EncounterChance> _tile_components;
    Components<Wall, Door> _wall_components;
};

class FloorComponentHandler : public TileComponentActionHandler {
public:
    void handleComponentSelection(Blueprint *blueprint) override {
        blueprint->renderTileComponentSelection<Floor>();
    }

    void handleComponentPlacement(Blueprint *blueprint, editor::MapPositionSelected *positions) override {
        blueprint->enqueueTileComponentPlacement<Floor>(&positions->positions);
    }
};

class WalkabilityComponentHandler : public TileComponentActionHandler {
public:
    void handleComponentSelection(Blueprint *blueprint) override {
        blueprint->renderTileComponentSelection<Walkability>();
    }

    void handleComponentPlacement(Blueprint *blueprint, editor::MapPositionSelected *positions) override {
        blueprint->enqueueTileComponentPlacement<Walkability>(&positions->positions);
    }
};

class EncounterChanceComponentHandler : public TileComponentActionHandler {
public:
    void handleComponentSelection(Blueprint *blueprint) override {
        blueprint->renderTileComponentSelection<EncounterChance>();
    }

    void handleComponentPlacement(Blueprint *blueprint, editor::MapPositionSelected *positions) override {
        blueprint->enqueueTileComponentPlacement<EncounterChance>(&positions->positions);
    }
};

class WallComponentHandler : public WallComponentActionHandler {
public:
    void handleComponentSelection(Blueprint *blueprint) override {
        blueprint->renderWallComponentSelection<Wall>();
    }

    void handleComponentPlacement(Blueprint *blueprint, editor::WallSelected *positions) override {

        blueprint->enqueueWallComponentPlacement<Wall>(&positions->positions);
    }
};

class DoorComponentHandler : public WallComponentActionHandler {
public:
    void handleComponentSelection(Blueprint *blueprint) override {
        blueprint->renderWallComponentSelection<Door>();
    }

    void handleComponentPlacement(Blueprint *blueprint, editor::WallSelected *positions) override {
        blueprint->enqueueWallComponentPlacement<Door>(&positions->positions);
    }
};
#endif//DUNGEON_CRAWLER_BLUEPRINT_HPP
