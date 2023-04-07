//
// Created by Patryk Szczypień on 03/04/2023.
//
#include <editor_application.hpp>

void EditorApplication::run() const {
    InitWindow(static_cast<int>(_window_width), static_cast<int>(_window_height), "Dungeon Crawler Level Editor");
    rlImGuiSetup(true);
    std::shared_ptr<Core> core = std::make_shared<Core>();
    core->registry.ctx().emplace<MapView>(core, Rectangle{0, 0, 1024, 768});
    core->registry.ctx().emplace<CurrentEditMode>(EditMode::None);
    MainView main_view{core};

    // main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();
        main_view.render();
        rlImGuiEnd();
        EndDrawing();
        core->dispatcher.update();
    }
    rlImGuiShutdown();
    CloseWindow();
}