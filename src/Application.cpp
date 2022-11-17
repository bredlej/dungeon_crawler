//
// Created by geoco on 17.11.2022.
//
#include <Application.hpp>

void run_raylib(const std::unique_ptr<UIView> &view) {
    InitWindow(Config::window.width, Config::window.height, Config::title.data());
    SetTargetFPS(144);
    while (!WindowShouldClose()) {
        view->update();
        view->render();
    }
    CloseWindow();
}

void initialize_views(ViewMap &views) {
    views[ViewMode::MainMenu] = std::make_unique<MainMenu>(MainMenu());
}

void Application::run() {
    std::printf("Running app.");

    initialize_views(_views);

    run_raylib(_views[_view_mode]);
}