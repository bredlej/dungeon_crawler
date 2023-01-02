//
// Created by geoco on 17.11.2022.
//

#ifndef DUNGEON_CRAWLER_CORE_HPP
#define DUNGEON_CRAWLER_CORE_HPP
#include "entt/entt.hpp"
#include <utility>
#include "assets.hpp"

class Core {
public:
    explicit Core() {
        std::printf("Core created\n");
    }

    void load_assets();
    assets::Assets *get_assets() {return _assets.get();}
    entt::dispatcher _dispatcher;
    entt::registry _registry;
private:
    std::unique_ptr<assets::Assets> _assets{nullptr};
};

class UIView {
public:
    explicit UIView(std::shared_ptr<Core> &core) : _core{core} {};
    virtual void render() = 0;
    virtual void update() = 0;
    virtual ~UIView() = default;

protected:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_CORE_HPP
