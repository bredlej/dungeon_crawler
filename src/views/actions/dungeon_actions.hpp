//
// Created by Patryk Szczypień on 23/03/2023.
//

#ifndef DUNGEON_CRAWLER_DUNGEON_ACTIONS_HPP
#define DUNGEON_CRAWLER_DUNGEON_ACTIONS_HPP
#include "level.hpp"
#include <components.hpp>
#include <core.hpp>
#include <events.hpp>
#include <memory>
#include <tile_map.hpp>
#include <wall_map.hpp>

class DungeonActions {
public:
    explicit DungeonActions(const std::shared_ptr<Core>& core, const TileMap *tile_map, const WallMap *wall_map) :_core{core}, _tile_map{tile_map}, _wall_map{wall_map} {_initialize();};
    explicit DungeonActions(std::shared_ptr<Core> sharedPtr, Level *pLevel) : _core{std::move(sharedPtr)}, _tile_map{&pLevel->tile_map}, _wall_map{&pLevel->wall_map} {_initialize();};
    DungeonActions(const DungeonActions &) noexcept = delete;
    DungeonActions(DungeonActions &) noexcept = delete;
    DungeonActions(DungeonActions &&other) noexcept = delete;
    DungeonActions &operator=(const DungeonActions &) noexcept = delete;
    DungeonActions &operator=(const DungeonActions &&) noexcept = delete;

    void turn_left();
    void turn_right();
    void move_forward();
    void move_back();
    void move_left();
    void move_right();
    void start_encounter();
private:
    void _initialize();
    void _on_movement(const events::dungeon::Movement &movement);
    void _on_encounter_chance_changed(events::dungeon::EncounterChanceChange);
    void _on_open_door(const events::dungeon::OpenDoor &open_door_event);
    const TileMap *_tile_map;
    const WallMap *_wall_map;
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_DUNGEON_ACTIONS_HPP
