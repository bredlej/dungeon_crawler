//
// Created by Patryk Szczypień on 25/08/2024.
//
#include <party_view.hpp>

void PartyView::render() noexcept {
    BeginDrawing();
    BeginTextureMode(_canvas);
    ClearBackground(BACKGROUND_COLOR);

    float attributes_start_height = 30;
    if (auto selected = _core->registry.ctx().find<components::view::party::Selected>()) {
        if (auto party_member = _core->registry.try_get<components::party::PartyMember>(selected->party_member)) {
            auto attributes = party_member->attributes;
            if (assets::Assets *assets = _core->get_assets()) {
                DrawTexture(assets->_textures._roles[party_member->role].get(), 10, 30, palette::whiteish);
            }
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::LEVEL])).c_str(), Vector2{170, attributes_start_height}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::HIT_POINTS])).c_str(), Vector2{170, attributes_start_height + 15}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::SKILL_POINTS])).c_str(), Vector2{170, attributes_start_height + 30}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::STRENGTH])).c_str(), Vector2{170, attributes_start_height + 45}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::DEXTERITY])).c_str(), Vector2{170, attributes_start_height + 60}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::INTELLIGENCE])).c_str(), Vector2{170, attributes_start_height + 75}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::WISDOM])).c_str(), Vector2{170, attributes_start_height + 90}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            DrawTextEx(_core->get_assets()->fonts.font, std::to_string(static_cast<int>(attributes.attributes[types::character::Attribute::LUCK])).c_str(), Vector2{170, attributes_start_height + 105}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
            std::string name = party_member->name;
            name.append(" the ").append(types::character::role_to_string[party_member->role]);
            DrawTextEx(_core->get_assets()->fonts.font, name.c_str(), Vector2{170, 10}, _core->get_assets()->fonts.font.baseSize, 1, palette::col_1);
        }
    }

    DrawTextEx(_core->get_assets()->fonts.font, "LVL", Vector2{130, attributes_start_height}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "HP", Vector2{130, attributes_start_height + 15}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "SP", Vector2{130, attributes_start_height + 30}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "STR", Vector2{130, attributes_start_height + 45}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "DEX", Vector2{130, attributes_start_height + 60}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "INT", Vector2{130, attributes_start_height + 75}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "WIS", Vector2{130, attributes_start_height + 90}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    DrawTextEx(_core->get_assets()->fonts.font, "LCK", Vector2{130, attributes_start_height + 105}, _core->get_assets()->fonts.font.baseSize, 1, palette::whiteish);
    EndTextureMode();
    static Rectangle POV_DIMENSION_FULLSCREEN = Rectangle{0, 0, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())), static_cast<float>(GetMonitorHeight(GetCurrentMonitor()))};
    static Rectangle GUI_DIMENSION_FULLSCREEN = Rectangle{static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.75f, static_cast<float>(GetMonitorHeight(GetCurrentMonitor())) * 0.1f, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.2f, static_cast<float>(GetMonitorWidth(GetCurrentMonitor())) * 0.2f};

    DrawTexturePro(_canvas.texture,
                   Rectangle{0.0f, 0.0f, (float) _canvas.texture.width, (float) -_canvas.texture.height},
                   IsWindowFullscreen() ? POV_DIMENSION_FULLSCREEN : POV_DIMENSION,
                   Vector2{0, 0},
                   0.0f,
                   WHITE);
    EndDrawing();
}

void PartyView::update() noexcept {
    if (IsKeyPressed(KEY_RIGHT)) {
        _core->dispatcher.trigger(events::ui::party::NextPartyMember{});
    } else if (IsKeyPressed(KEY_LEFT)) {
        _core->dispatcher.trigger(events::ui::party::PreviousPartyMember{});
    }
}

void PartyView::next_party_member() {
    if (auto *party = _core->registry.ctx().find<components::party::Party>()) {
        if (auto selected = _core->registry.ctx().find<components::view::party::Selected>()) {
            auto it = std::find(party->members.begin(), party->members.end(), selected->party_member);
            if (it != party->members.end()) {
                if (std::next(it) != party->members.end()) {
                    show(*std::next(it));
                } else {
                    show(party->members.front());
                }
            }
        }
    }
}

void PartyView::previous_party_member() {
    if (auto *party = _core->registry.ctx().find<components::party::Party>()) {
        if (auto selected = _core->registry.ctx().find<components::view::party::Selected>()) {
            auto it = std::find(party->members.begin(), party->members.end(), selected->party_member);
            if (it != party->members.end()) {
                if (it != party->members.begin()) {
                    show(*std::prev(it));
                } else {
                    show(party->members.back());
                }
            }
        }
    }
}

void PartyView::show(entt::entity party_member) noexcept {
    _core->registry.ctx().erase<components::view::party::Selected>();
    _core->registry.ctx().emplace<components::view::party::Selected>(party_member);
}