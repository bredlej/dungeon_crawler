//
// Created by geoco on 17.11.2022.
//
#include <application.hpp>

void initialize_player(const std::shared_ptr<Core>& core) {
    const entt::entity player_entity = core->registry.create();
    core->registry.emplace<components::general::Player>(player_entity, true);
    core->registry.emplace<components::general::Direction>(player_entity, WorldDirection::EAST);
    core->registry.emplace<components::fields::MapPosition>(player_entity, 1, 1);
}

static inline void setup_imgui_colors() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ImVec4(static_cast<float>(palette::col_18.r)/255.0f, static_cast<float>(palette::col_18.g)/255.0f, static_cast<float>(palette::col_18.b)/255.0f, 1);
    colors[ImGuiCol_WindowBg] = ImVec4(static_cast<float>(palette::col_11.r)/255.0f, static_cast<float>(palette::col_11.g)/255.0f, static_cast<float>(palette::col_11.b)/255.0f, 1);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(static_cast<float>(palette::col_11.r)/255.0f, static_cast<float>(palette::col_11.g)/255.0f, static_cast<float>(palette::col_11.b)/255.0f, 1);
    colors[ImGuiCol_PopupBg]                = ImVec4(static_cast<float>(palette::col_11.r)/255.0f, static_cast<float>(palette::col_11.g)/255.0f, static_cast<float>(palette::col_11.b)/255.0f, 1);
    colors[ImGuiCol_Border]                 = ImVec4(static_cast<float>(palette::col_10.r)/255.0f, static_cast<float>(palette::col_10.g)/255.0f, static_cast<float>(palette::col_10.b)/255.0f, 1);
    colors[ImGuiCol_BorderShadow]           = ImVec4(static_cast<float>(palette::col_9.r)/255.0f, static_cast<float>(palette::col_9.g)/255.0f, static_cast<float>(palette::col_9.b)/255.0f, 1);
    colors[ImGuiCol_FrameBg]                = ImVec4(static_cast<float>(palette::col_11.r)/255.0f, static_cast<float>(palette::col_11.g)/255.0f, static_cast<float>(palette::col_11.b)/255.0f, 1);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(static_cast<float>(palette::col_11.r)/255.0f, static_cast<float>(palette::col_11.g)/255.0f, static_cast<float>(palette::col_11.b)/255.0f, 1);
    colors[ImGuiCol_TitleBg]                = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(static_cast<float>(palette::col_10.r)/255.0f, static_cast<float>(palette::col_10.g)/255.0f, static_cast<float>(palette::col_10.b)/255.0f, 1);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(static_cast<float>(palette::col_4.r)/255.0f, static_cast<float>(palette::col_4.g)/255.0f, static_cast<float>(palette::col_4.b)/255.0f, 1);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(static_cast<float>(palette::col_9.r)/255.0f, static_cast<float>(palette::col_9.g)/255.0f, static_cast<float>(palette::col_4.b)/255.0f, 1);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(static_cast<float>(palette::col_6.r)/255.0f, static_cast<float>(palette::col_6.g)/255.0f, static_cast<float>(palette::col_6.b)/255.0f, 1);
    colors[ImGuiCol_CheckMark]              = ImVec4(static_cast<float>(palette::col_5.r)/255.0f, static_cast<float>(palette::col_5.g)/255.0f, static_cast<float>(palette::col_5.b)/255.0f, 1);
    colors[ImGuiCol_SliderGrab]             = ImVec4(static_cast<float>(palette::col_5.r)/255.0f, static_cast<float>(palette::col_5.g)/255.0f, static_cast<float>(palette::col_5.b)/255.0f, 1);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_Button]                 = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(static_cast<float>(palette::col_4.r)/255.0f, static_cast<float>(palette::col_4.g)/255.0f, static_cast<float>(palette::col_4.b)/255.0f, 1);
    colors[ImGuiCol_ButtonActive]           = ImVec4(static_cast<float>(palette::col_6.r)/255.0f, static_cast<float>(palette::col_6.g)/255.0f, static_cast<float>(palette::col_6.b)/255.0f, 1);
    colors[ImGuiCol_Header]                 = ImVec4(static_cast<float>(palette::col_10.r)/255.0f, static_cast<float>(palette::col_10.g)/255.0f, static_cast<float>(palette::col_10.b)/255.0f, 1);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_HeaderActive]           = ImVec4(static_cast<float>(palette::col_5.r)/255.0f, static_cast<float>(palette::col_5.g)/255.0f, static_cast<float>(palette::col_5.b)/255.0f, 1);
    colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(static_cast<float>(palette::col_11.r)/255.0f, static_cast<float>(palette::col_11.g)/255.0f, static_cast<float>(palette::col_11.b)/255.0f, 1);
    colors[ImGuiCol_TabHovered]             = ImVec4(static_cast<float>(palette::col_5.r)/255.0f, static_cast<float>(palette::col_5.g)/255.0f, static_cast<float>(palette::col_5.b)/255.0f, 1);
    colors[ImGuiCol_TabActive]              = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(static_cast<float>(palette::col_8.r)/255.0f, static_cast<float>(palette::col_8.g)/255.0f, static_cast<float>(palette::col_8.b)/255.0f, 1);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Application::_toggle_fullscreen() {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {
        if (IsWindowFullscreen())
        {
            ToggleFullscreen();
            SetWindowSize(Config::window.width, Config::window.height);
        }
        else
        {
            SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
            ToggleFullscreen();
        }
    }
}

void Application::run() {
    std::printf("Dungeon crawler is running.\n");

    setup_imgui_colors();
    _core->load_assets();

    initialize_player(_core);

    SetTargetFPS(144);
    while (!WindowShouldClose()) {
        _toggle_fullscreen();
        _core->dispatcher.update();
        switch (_view_mode) {
            case ViewMode::MainMenu:
                main_menu_view->update();
                main_menu_view->render();
                break;
            case ViewMode::Dungeon:
                dungeon_view->update();
                dungeon_view->render();
                break;
        }
    }

}