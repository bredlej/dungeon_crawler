//
// Created by geoco on 17.11.2022.
//
#include <application.hpp>

void initialize_views(ViewMap &views, const std::shared_ptr<Core> &core) {
    views[ViewMode::MainMenu] = std::make_unique<MainMenu>(MainMenu(core));
    views[ViewMode::Dungeon] = std::make_unique<DungeonView>(core, TileMap(20, 20));
}

void run_raylib(ViewMap &views, ViewMode view_mode, const std::shared_ptr<Core> &core) {
    InitWindow(Config::window.width, Config::window.height, Config::title.data());
    initialize_views(views, core);
    core->load_assets();
    SetTargetFPS(144);
    while (!WindowShouldClose()) {
        views[view_mode]->update();
        views[view_mode]->render();
    }

    CloseWindow();
}

void Application::run() {
    std::printf("Running app.\n");

    run_raylib(_views, _view_mode, _core);
}