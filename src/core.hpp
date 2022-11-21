//
// Created by geoco on 17.11.2022.
//

#ifndef DUNGEON_CRAWLER_CORE_HPP
#define DUNGEON_CRAWLER_CORE_HPP
#include <entt/entt.hpp>
#include <utility>

class Core {
public:
    const entt::dispatcher &get_dispatcher() {
        return _dispatcher;
    }
    void invoke() {
        i+=1;
    }
    int get_i() const {
        return i;
    }
private:
    entt::dispatcher _dispatcher;
    entt::registry _registry;
    int i = 0;
};

class UIView {
public:
    explicit UIView(std::shared_ptr<Core> core) : _core{std::move(core)} {};
    virtual void render() = 0;
    virtual void update() = 0;
    virtual ~UIView() = default;

protected:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_CORE_HPP
