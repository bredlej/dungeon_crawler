//
// Created by geoco on 17.11.2022.
//
#include <application.hpp>

void run_raylib(const std::unique_ptr<UIView> &view, const std::shared_ptr<Core> &core) {
    InitWindow(Config::window.width, Config::window.height, Config::title.data());
    core->load_assets();
    SetTargetFPS(144);
    while (!WindowShouldClose()) {
        view->update();
        view->render();
    }

    CloseWindow();
}

void initialize_views(ViewMap &views, const std::shared_ptr<Core> &core) {
    views[ViewMode::MainMenu] = std::make_unique<MainMenu>(MainMenu(core));
    views[ViewMode::Dungeon] = std::make_unique<DungeonView>(core, TileMap(20, 20));
}

void Application::run() {
    std::printf("Running app.\n");

    initialize_views(_views, _core);
    run_raylib(_views[_view_mode], _core);
}