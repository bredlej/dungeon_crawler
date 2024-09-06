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
#include <memory>
#include <scheduler.hpp>
#include <colors.hpp>

struct ModXY {
    int32_t x;
    int32_t y;
};

constexpr static Color BACKGROUND_COLOR = {0x34 /2, 0x31/2, 0x1d/2, 0xff};
constexpr static Rectangle POV_DIMENSION = Rectangle{0.0f, 0.0f, static_cast<float>(1024.0f), static_cast<float>(768.0f)};
constexpr static Rectangle GUI_DIMENSION = Rectangle{750.0f, 50.0f, static_cast<float>(240.0f), static_cast<float>(240.0f)};

class Core {
public:
    explicit Core() {
        std::printf("Core created\n");
    }

    template<typename COMPONENT>
    COMPONENT* try_get(entt::entity entity) {return registry.try_get<COMPONENT>(entity); }

    void load_assets();
    assets::Assets *get_assets() {return _assets.get();}
    entt::dispatcher dispatcher;
    entt::registry registry;
    entt::scheduler scheduler;
    GameLog game_log;
    pcg32 pcg;
private:
    std::unique_ptr<assets::Assets> _assets{nullptr};
};

template <typename T>
class UIView {
public:
    static constexpr auto RENDER_TEXTURE_WIDTH = 320;
    static constexpr auto RENDER_TEXTURE_HEIGHT = 240;

    explicit UIView(std::shared_ptr<Core> &core) : _core{core} {};
    virtual void render() noexcept{
        static_cast<T*>(this)->render();
    };
    virtual void update() noexcept {
        static_cast<T*>(this)->update();
    }
    virtual ~UIView() = default;

    void _render_texture(const Texture &texture, const Rectangle &fullscreen_dimension) const {
        DrawTexturePro(texture,
                       Rectangle{0.0f, 0.0f, (float) texture.width, (float) -texture.height},
                       IsWindowFullscreen() ? fullscreen_dimension : POV_DIMENSION,
                       Vector2{0, 0},
                       0.0f,
                       WHITE);
    }
protected:
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_CORE_HPP
