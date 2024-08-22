//
// Created by Patryk Szczypień on 16/08/2024.
//

#ifndef DUNGEON_CRAWLER_ENCOUNTER_VIEW_HPP
#define DUNGEON_CRAWLER_ENCOUNTER_VIEW_HPP

#include <engine/core.hpp>
#include <engine/events.hpp>

class EncounterView : public UIView<EncounterView> {
public:
    explicit EncounterView(std::shared_ptr<Core> core, BattleDirector *battle_director)
        : UIView{core}, _battle_director{battle_director} {}

    EncounterView(const EncounterView &) noexcept = delete;
    EncounterView(EncounterView &) noexcept = delete;
    EncounterView(EncounterView &&other) noexcept = default;
    EncounterView &operator=(const EncounterView &) noexcept = delete;
    EncounterView &operator=(const EncounterView &&) noexcept = delete;

    void render() noexcept override;
    void update() noexcept override;

    ~EncounterView() override = default;
private:
    BattleDirector *_battle_director;
};
#endif//DUNGEON_CRAWLER_ENCOUNTER_VIEW_HPP
