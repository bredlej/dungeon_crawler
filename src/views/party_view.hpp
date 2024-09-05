//
// Created by Patryk Szczypień on 25/08/2024.
//

#ifndef DUNGEON_CRAWLER_PARTY_VIEW_HPP
#define DUNGEON_CRAWLER_PARTY_VIEW_HPP

#include <engine/core.hpp>
#include <engine/events.hpp>
#include <ecs/components.hpp>
#include <entt/entt.hpp>

class PartyView : public UIView<PartyView> {
public:
    explicit PartyView(std::shared_ptr<Core> core) : UIView{core}, _canvas(LoadRenderTexture(320, 240)) {
        _core->dispatcher.sink<events::ui::party::NextPartyMember>().connect<&PartyView::next_party_member>(this);
        _core->dispatcher.sink<events::ui::party::PreviousPartyMember>().connect<&PartyView::previous_party_member>(this);
    }
    void show(entt::entity) noexcept;

    void render() noexcept override;
    void update() noexcept override;

    void next_party_member();
    void previous_party_member();

    ~PartyView() override = default;
private:
    RenderTexture _canvas;
};
#endif//DUNGEON_CRAWLER_PARTY_VIEW_HPP
