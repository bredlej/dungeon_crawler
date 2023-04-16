//
// Created by Patryk Szczypień on 08/04/2023.
//

#ifndef DUNGEON_CRAWLER_ENTITY_DETAILS_HPP
#define DUNGEON_CRAWLER_ENTITY_DETAILS_HPP
#include <core.hpp>
#include <events.hpp>
#include <components.hpp>
#include <fmt/format.h>
#include <component_renderer.hpp>
class EntityDetails {
public:
    explicit EntityDetails(std::shared_ptr<Core> core) : _core{core}, _component_renderer{core} {};
    EntityDetails(const EntityDetails &) = delete;
    EntityDetails(EntityDetails &&) = default;
    EntityDetails &operator=(const EntityDetails &) = delete;
    EntityDetails &operator=(EntityDetails &&) = delete;
    ~EntityDetails() = default;
    void render();
    void update();
private:
    bool _visible{true};
    ComponentRenderer _component_renderer;
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_ENTITY_DETAILS_HPP
