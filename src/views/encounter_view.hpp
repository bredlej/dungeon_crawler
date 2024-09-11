//
// Created by Patryk Szczypień on 16/08/2024.
//

#ifndef DUNGEON_CRAWLER_ENCOUNTER_VIEW_HPP
#define DUNGEON_CRAWLER_ENCOUNTER_VIEW_HPP

#include <engine/battle_system/battle_director.hpp>
#include <engine/core.hpp>
#include <engine/events.hpp>
#include <views/ui/encounter_ui.hpp>

class EncounterView : public UIView<EncounterView> {
public:
    explicit EncounterView(std::shared_ptr<Core> core, BattleDirector *battle_director)
        : UIView{core}, _battle_director{battle_director}, _ui{core}, _canvas(LoadRenderTexture(RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT))
    {
        _initialize();
    }

    EncounterView(const EncounterView &) noexcept = delete;
    EncounterView(EncounterView &) noexcept = delete;
    EncounterView(EncounterView &&other) noexcept = delete;
    EncounterView &operator=(const EncounterView &) noexcept = delete;
    EncounterView &operator=(const EncounterView &&) noexcept = delete;

    void render() noexcept override;
    void update() noexcept override;

    ~EncounterView() override = default;

private:
    EncounterUI _ui;
    RenderTexture _canvas;

    entt::entity _selected_enemy{entt::null};
    std::vector<Color> _empty_canvas;
    float _timer {0.0f};

    void _initialize() noexcept;
    void _clear_canvas() noexcept;
    void _render_enemies() noexcept;
    void _render_effects() noexcept;
    void _render_enemy(entt::entity entity, int x, int y) noexcept;
    void _render_enemy_slot(auto idx, const auto &row_entities, auto row_start_x, auto row_y, auto offset);
    entt::entity _get_entity_at_point(auto idx, const auto &row_entities, auto row_start_x, auto row_y, auto offset, auto mouse_x, auto mouse_y);
    void _render_mouseover() noexcept;
    BattleDirector *_battle_director;
};
#endif//DUNGEON_CRAWLER_ENCOUNTER_VIEW_HPP
