//
// Created by Patryk Szczypień on 14/04/2023.
//

#ifndef DUNGEON_CRAWLER_COMPONENT_RENDERER_HPP
#define DUNGEON_CRAWLER_COMPONENT_RENDERER_HPP
#include <core.hpp>
#include <components.hpp>
#include <fmt/format.h>

class ComponentRenderer {
public:
    explicit ComponentRenderer(std::shared_ptr<Core> core) : _core{core} {};
    ComponentRenderer(const ComponentRenderer &) = delete;
    ComponentRenderer(ComponentRenderer &&) = default;
    ComponentRenderer &operator=(const ComponentRenderer &) = delete;
    ComponentRenderer &operator=(ComponentRenderer &&) = delete;
    ~ComponentRenderer() = default;
    template<typename T>
    void render_component(entt::entity entity);
    template<typename T>
    void render_component(T &component);
private:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_COMPONENT_RENDERER_HPP
