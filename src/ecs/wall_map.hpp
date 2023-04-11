//
// Created by geoco on 29.12.2022.
//

#ifndef DUNGEON_CRAWLER_WALL_MAP_HPP
#define DUNGEON_CRAWLER_WALL_MAP_HPP
#include "tile_map.hpp"
#include <cstdint>
#include <ecs/components.hpp>
#include <engine/core.hpp>
#include <entt/entt.hpp>
#include <utility>

struct WallEntity {
    entt::entity entity {entt::null};
};
class WallMap {
public:
    explicit WallMap(std::shared_ptr<Core> &core) : _core(core), _walls{std::vector<WallEntity>()} {};
    WallMap(const WallMap &other) noexcept = delete;
    WallMap(WallMap &other) noexcept = delete;
    WallMap(WallMap &&other) noexcept = default;
    WallMap &operator=(const WallMap &other) noexcept = delete;
    std::vector<WallEntity> _walls;
    [[nodiscard]] entt::entity get_between(components::fields::MapPosition field1, components::fields::MapPosition field2) const noexcept;
    void initialize(const TileMap&);
    void place_wall_at(const TileMap &tile_map, int32_t x, int32_t y, WallType wall_type);
    void from_json(const TileMap &tile_map, const nlohmann::json &json);
    void to_json(nlohmann::json &json);

private:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_WALL_MAP_HPP
