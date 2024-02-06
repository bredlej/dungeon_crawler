//
// Created by Patryk Szczypień on 03/04/2023.
//
#include <editor_application.hpp>
#include <skills.hpp>

/**
 * @brief Executes the main game loop for the EditorApplication.
 *
 * This method initializes the window and sets up ImGui. It creates a Core object and sets up the MapView
 * and EditorAssets components. Then, it enters a loop where it updates and renders the MainView, handles
 * ImGui rendering, and updates the Core dispatcher. The loop continues until the window is closed by the
 * user or the ESC key is pressed.
 *
 * @note This method does not return.
 *
 * @see EditorApplication::InitWindow
 * @see EditorApplication::rlImGuiSetup
 * @see Core
 * @see MapView
 * @see EditorAssets
 * @see MainView
 *
 */
LaunchResult EditorApplication::run() const {
    std::ifstream file(".config.json");
    auto json = nlohmann::json::parse(file);
    if (json.empty()) {
        return LaunchResult{false, "Could not find .config.json"};
    }
    if (json["game_path"].empty()) {
        return LaunchResult{false, "Could not find game_path in .config.json"};
    }
    std::string game_path = json["game_path"];
    std::printf("%s\n", game_path.c_str());
    InitWindow(static_cast<int>(_window_width), static_cast<int>(_window_height), "Dungeon Crawler Level Editor");
    rlImGuiSetup(true);
    std::shared_ptr<Core> core = std::make_shared<Core>();
    core->registry.ctx().emplace<Path>(json["game_path"]);
    core->registry.ctx().emplace<MapView>(core, Rectangle{0, 0, 1024, 768});
    MainView main_view{core};

    core->registry.ctx().emplace<EditorAssets>(game_path);
    core->registry.ctx().find<EditorAssets>()->load_textures();

    skills::SkillsMap skills_map = skills::SkillsMap::from_json(SkillParser::parse(game_path + "assets/Skills/skills.json"));
    core->registry.ctx().emplace<skills::SkillsMap>(skills_map);
    SetExitKey(0);
    // main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();
        main_view.update();
        main_view.render();
        rlImGuiEnd();
        EndDrawing();
        core->dispatcher.update();
    }
    rlImGuiShutdown();
    CloseWindow();
    return LaunchResult{true, ""};
}