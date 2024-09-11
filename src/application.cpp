//
// Created by geoco on 17.11.2022.
//
#include "encounter_view.hpp"
#include "skills.hpp"
#include <application.hpp>

void initialize_player(const std::shared_ptr<Core> &core) {
    const entt::entity player_entity = core->registry.create();
    core->registry.emplace<components::general::Player>(player_entity, true);
    core->registry.emplace<components::general::Direction>(player_entity, WorldDirection::EAST);
    core->registry.emplace<components::tiles::MapPosition>(player_entity, 1, 1);

    const components::battle::Attributes attributes_mage{
            std::unordered_map<types::character::Attribute, float>{
                    {types::character::Attribute::LEVEL, 1.0f},
                    {types::character::Attribute::HIT_POINTS, 13.0f},
                    {types::character::Attribute::SKILL_POINTS, 15.0f},
                    {types::character::Attribute::STRENGTH, 8.0f},
                    {types::character::Attribute::INTELLIGENCE, 11.0f},
                    {types::character::Attribute::VITALITY, 9.0f},
                    {types::character::Attribute::WISDOM, 13.0f},
                    {types::character::Attribute::DEXTERITY, 7.0f},
                    {types::character::Attribute::LUCK, 10.0f},
                    {types::character::Attribute::ATTACK, 7.0f},
                    {types::character::Attribute::MAGIC_ATTACK, 12.0f},
                    {types::character::Attribute::DEFENSE, 7.0f},
                    {types::character::Attribute::MAGIC_DEFENSE, 11.0f}
            }};

    const components::battle::Attributes attributes_warrior{
            std::unordered_map<types::character::Attribute, float>{
                    {types::character::Attribute::LEVEL, 1.0f},
                    {types::character::Attribute::HIT_POINTS, 15.0f},
                    {types::character::Attribute::SKILL_POINTS, 11.0f},
                    {types::character::Attribute::STRENGTH, 13.0f},
                    {types::character::Attribute::INTELLIGENCE, 7.0f},
                    {types::character::Attribute::VITALITY, 11.0f},
                    {types::character::Attribute::WISDOM, 7.0f},
                    {types::character::Attribute::DEXTERITY, 9.0f},
                    {types::character::Attribute::LUCK, 10.0f},
                    {types::character::Attribute::ATTACK, 13.0f},
                    {types::character::Attribute::MAGIC_ATTACK, 6.0f},
                    {types::character::Attribute::DEFENSE, 12.0f},
                    {types::character::Attribute::MAGIC_DEFENSE, 5.0f}
            }};

    const components::battle::Attributes attributes_priest{
            std::unordered_map<types::character::Attribute, float>{
                    {types::character::Attribute::LEVEL, 1.0f},
                    {types::character::Attribute::HIT_POINTS, 9.0f},
                    {types::character::Attribute::SKILL_POINTS, 13.0f},
                    {types::character::Attribute::STRENGTH, 6.0f},
                    {types::character::Attribute::INTELLIGENCE, 9.0f},
                    {types::character::Attribute::VITALITY, 8.0f},
                    {types::character::Attribute::WISDOM, 14.0f},
                    {types::character::Attribute::DEXTERITY, 8.0f},
                    {types::character::Attribute::LUCK, 10.0f},
                    {types::character::Attribute::ATTACK, 6.0f},
                    {types::character::Attribute::MAGIC_ATTACK, 10.0f},
                    {types::character::Attribute::DEFENSE, 5.0f},
                    {types::character::Attribute::MAGIC_DEFENSE, 9.0f}
            }};

    const entt::entity party_mage = core->registry.create();
    core->registry.emplace<components::party::PartyMember>(party_mage, "Bred", types::character::Role::MAGE, attributes_mage);

    const entt::entity party_warrior = core->registry.create();
    core->registry.emplace<components::party::PartyMember>(party_warrior, "Kain", types::character::Role::WARRIOR, attributes_warrior);

    const entt::entity party_priest = core->registry.create();
    core->registry.emplace<components::party::PartyMember>(party_priest, "Luna", types::character::Role::CULTIST, attributes_priest);

    const entt::entity party_entity = core->registry.create();
    const components::party::Party party {
        {party_mage, party_warrior, party_priest}
    };
    core->registry.ctx().emplace<components::party::Party>(party);
    core->registry.ctx().emplace<components::view::party::Selected>(party_mage);
}

static inline void setup_imgui_colors() {
    ImGuiStyle *style = &ImGui::GetStyle();
    ImVec4 *colors = style->Colors;
    colors[ImGuiCol_Text] = ImVec4(static_cast<float>(palette::col_18.r) / 255.0f, static_cast<float>(palette::col_18.g) / 255.0f, static_cast<float>(palette::col_18.b) / 255.0f, 1);
    colors[ImGuiCol_WindowBg] = ImVec4(static_cast<float>(palette::col_11.r) / 255.0f, static_cast<float>(palette::col_11.g) / 255.0f, static_cast<float>(palette::col_11.b) / 255.0f, 1);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(static_cast<float>(palette::col_11.r) / 255.0f, static_cast<float>(palette::col_11.g) / 255.0f, static_cast<float>(palette::col_11.b) / 255.0f, 1);
    colors[ImGuiCol_PopupBg] = ImVec4(static_cast<float>(palette::col_11.r) / 255.0f, static_cast<float>(palette::col_11.g) / 255.0f, static_cast<float>(palette::col_11.b) / 255.0f, 1);
    colors[ImGuiCol_Border] = ImVec4(static_cast<float>(palette::col_10.r) / 255.0f, static_cast<float>(palette::col_10.g) / 255.0f, static_cast<float>(palette::col_10.b) / 255.0f, 1);
    colors[ImGuiCol_BorderShadow] = ImVec4(static_cast<float>(palette::col_9.r) / 255.0f, static_cast<float>(palette::col_9.g) / 255.0f, static_cast<float>(palette::col_9.b) / 255.0f, 1);
    colors[ImGuiCol_FrameBg] = ImVec4(static_cast<float>(palette::col_11.r) / 255.0f, static_cast<float>(palette::col_11.g) / 255.0f, static_cast<float>(palette::col_11.b) / 255.0f, 1);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_FrameBgActive] = ImVec4(static_cast<float>(palette::col_11.r) / 255.0f, static_cast<float>(palette::col_11.g) / 255.0f, static_cast<float>(palette::col_11.b) / 255.0f, 1);
    colors[ImGuiCol_TitleBg] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_TitleBgActive] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_MenuBarBg] = ImVec4(static_cast<float>(palette::col_10.r) / 255.0f, static_cast<float>(palette::col_10.g) / 255.0f, static_cast<float>(palette::col_10.b) / 255.0f, 1);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(static_cast<float>(palette::col_4.r) / 255.0f, static_cast<float>(palette::col_4.g) / 255.0f, static_cast<float>(palette::col_4.b) / 255.0f, 1);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(static_cast<float>(palette::col_9.r) / 255.0f, static_cast<float>(palette::col_9.g) / 255.0f, static_cast<float>(palette::col_4.b) / 255.0f, 1);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(static_cast<float>(palette::col_6.r) / 255.0f, static_cast<float>(palette::col_6.g) / 255.0f, static_cast<float>(palette::col_6.b) / 255.0f, 1);
    colors[ImGuiCol_CheckMark] = ImVec4(static_cast<float>(palette::col_5.r) / 255.0f, static_cast<float>(palette::col_5.g) / 255.0f, static_cast<float>(palette::col_5.b) / 255.0f, 1);
    colors[ImGuiCol_SliderGrab] = ImVec4(static_cast<float>(palette::col_5.r) / 255.0f, static_cast<float>(palette::col_5.g) / 255.0f, static_cast<float>(palette::col_5.b) / 255.0f, 1);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_Button] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_ButtonHovered] = ImVec4(static_cast<float>(palette::col_4.r) / 255.0f, static_cast<float>(palette::col_4.g) / 255.0f, static_cast<float>(palette::col_4.b) / 255.0f, 1);
    colors[ImGuiCol_ButtonActive] = ImVec4(static_cast<float>(palette::col_6.r) / 255.0f, static_cast<float>(palette::col_6.g) / 255.0f, static_cast<float>(palette::col_6.b) / 255.0f, 1);
    colors[ImGuiCol_Header] = ImVec4(static_cast<float>(palette::col_10.r) / 255.0f, static_cast<float>(palette::col_10.g) / 255.0f, static_cast<float>(palette::col_10.b) / 255.0f, 1);
    colors[ImGuiCol_HeaderHovered] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_HeaderActive] = ImVec4(static_cast<float>(palette::col_5.r) / 255.0f, static_cast<float>(palette::col_5.g) / 255.0f, static_cast<float>(palette::col_5.b) / 255.0f, 1);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(static_cast<float>(palette::col_11.r) / 255.0f, static_cast<float>(palette::col_11.g) / 255.0f, static_cast<float>(palette::col_11.b) / 255.0f, 1);
    colors[ImGuiCol_TabHovered] = ImVec4(static_cast<float>(palette::col_5.r) / 255.0f, static_cast<float>(palette::col_5.g) / 255.0f, static_cast<float>(palette::col_5.b) / 255.0f, 1);
    colors[ImGuiCol_TabActive] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_TabUnfocused] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(static_cast<float>(palette::col_8.r) / 255.0f, static_cast<float>(palette::col_8.g) / 255.0f, static_cast<float>(palette::col_8.b) / 255.0f, 1);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);// Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f); // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Application::_toggle_fullscreen() noexcept {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
        if (IsWindowFullscreen()) {
            ToggleFullscreen();
            SetWindowSize(Config::window.width, Config::window.height);
        } else {
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            ToggleFullscreen();
        }
    }
}

void Application::initialize() noexcept {
    main_menu_view = std::make_unique<MainMenu>(_core);
    _core->dispatcher.sink<events::dungeon::StartEncounter>().connect<&Application::start_encounter>(this);
    _core->dispatcher.sink<events::dungeon::EndEncounter>().connect<&Application::end_encounter>(this);
    _core->dispatcher.sink<events::ui::ShowPartyView>().connect<&Application::show_party_view>(this);
    _core->dispatcher.sink<events::ui::ShowDungeonView>().connect<&Application::show_dungeon_view>(this);

    _core->registry.ctx().emplace<skills::SkillsMap>(skills::SkillsMap::from_json(SkillParser::parse("assets/Skills/skills.json")));
}

void Application::run() noexcept {
    std::printf("Dungeon crawler is running.\n");

    setup_imgui_colors();
    _core->load_assets();

    initialize();
    initialize_player(_core);
    auto level = Level(_core, TileMap(_core, 20, 20));

    level.load("assets/Levels/Ruins/ruins0001.json");
    dungeon_view = std::make_unique<DungeonView>(_core, std::move(level));

    SetTargetFPS(144);

    Scheduler<std::chrono::milliseconds> scheduler(_core);
    scheduler.builder()
            .after(1, [](const std::shared_ptr<Core> &core) {
                core->scheduler.update(1);
            })
            .repeating()
            .run();

    while (!WindowShouldClose()) {
        _toggle_fullscreen();
        _core->dispatcher.update();
        common_update();
        switch (_view_mode) {
            case ViewMode::MainMenu:
                if (main_menu_view) {
                    main_menu_view->update();
                    main_menu_view->render();
                }
                break;
            case ViewMode::Dungeon:
                if (dungeon_view) {
                    dungeon_view->update();
                    dungeon_view->render();
                }
                break;
            case ViewMode::Encounter:
                if (encounter_view) {
                    encounter_view->update();
                    encounter_view->render();
                }
                break;
            case ViewMode::Party:
                if (party_view) {
                    party_view->update();
                    party_view->render();
                }
                break;
        }
    }
}

void Application::common_update() noexcept {
    if (IsKeyPressed(KEY_ONE)) {
        _core->dispatcher.trigger<events::ui::ShowDungeonView>();
    }
    if (IsKeyPressed(KEY_TWO)) {
        _core->dispatcher.trigger<events::ui::ShowPartyView>();
    }
    if (IsKeyPressed(KEY_F11)) {
        _toggle_fullscreen();
    }
}

void Application::show_party_view() noexcept {
    _view_mode = ViewMode::Party;
}

void Application::show_dungeon_view() noexcept {
    _view_mode = ViewMode::Dungeon;
}

void Application::start_encounter(events::dungeon::StartEncounter &event) noexcept {
    auto duration_ms = 150;
    auto bloom_alpha = 0.200f;
    auto brightness_threshold = 0.190f;
    auto brightness_step = (brightness_threshold / (float) duration_ms) * 2.0f;
    auto bloom_step = bloom_alpha / (float) duration_ms;

    if (auto *effects = _core->registry.ctx().find<components::values::ShaderEffects>()) {
        effects->bloom_enabled = true;
        effects->bloom_blend_mode = BLEND_ADD_COLORS;
        effects->bloom_alpha = bloom_alpha;
        effects->brightness_threshold = brightness_threshold;
        effects->blur_enabled = true;
        effects->blur_blend_mode = BLEND_MULTIPLIED;
    }

    _core->registry.ctx().emplace<components::values::AnimationTimer>((uint32_t) duration_ms);
    _core->scheduler.attach([this, brightness_step](auto delta, void *, auto succeed, auto fail) {
        if (auto *timer = _core->registry.ctx().find<components::values::AnimationTimer>()) {
            timer->counter -= 1;
            if (timer->counter <= 0) {
                _core->registry.ctx().erase<components::values::AnimationTimer>();
            }
            if (auto *effects = _core->registry.ctx().find<components::values::ShaderEffects>()) {
                effects->brightness_threshold += brightness_step;
            }
        } else {
            _core->registry.ctx().emplace<components::values::Encounter>();
            _view_mode = ViewMode::Encounter;
            _battle_director.apply_configuration(battle_configurations::animated_intro());
            _battle_director.apply_configuration(battle_configurations::enemies_in_two_rows<3>());
            succeed();
        }
    });
}

void Application::end_encounter(const events::dungeon::EndEncounter &event) noexcept {
    if (auto *effects = _core->registry.ctx().find<components::values::ShaderEffects>()) {
        effects->bloom_enabled = false;
        effects->blur_enabled = false;
    }
    _core->dispatcher.trigger(events::ui::ShowDungeonView());
}
