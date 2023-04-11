//
// Created by geoco on 17.11.2022.
//

#ifndef DUNGEON_CRAWLER_CORE_HPP
#define DUNGEON_CRAWLER_CORE_HPP
#include "entt/entt.hpp"
#include <utility>
#include "assets.hpp"
#include <game_log.hpp>
#include <pcg/pcg_random.hpp>
#include <cstdint>

struct ModXY {
    int32_t x;
    int32_t y;
};

class Core {
public:
    explicit Core() {
        std::printf("Core created\n");
    }

    void load_assets();
    assets::Assets *get_assets() {return _assets.get();}
    entt::dispatcher dispatcher;
    entt::registry registry;
    GameLog game_log;
    pcg32 pcg;
private:
    std::unique_ptr<assets::Assets> _assets{nullptr};
};

template <typename T>
class UIView {
public:
    explicit UIView(std::shared_ptr<Core> &core) : _core{core} {};
    virtual void render() noexcept{
        static_cast<T*>(this)->render();
    };
    virtual void update() noexcept {
        static_cast<T*>(this)->update();
    }
    virtual ~UIView() = default;

protected:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_CORE_HPP
