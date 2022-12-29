//
// Created by geoco on 17.11.2022.
//
#include <application.hpp>

void initialize_views(ViewMap &views,  std::shared_ptr<Core>& core) {
    views[ViewMode::MainMenu] = std::make_unique<MainMenu>(MainMenu(core));
    views[ViewMode::Dungeon] = std::make_unique<DungeonView>(core, TileMap(core, 20, 20));
}

void initialize_player(const std::shared_ptr<Core>& core) {
    const entt::entity player_entity = core->_registry.create();
    core->_registry.emplace<components::general::Player>(player_entity, true);
    core->_registry.emplace<components::general::Direction>(player_entity, WorldDirection::NORTH);
    core->_registry.emplace<components::fields::MapPosition>(player_entity, 5, 9);
}



void run_raylib(ViewMap &views, ViewMode view_mode, std::shared_ptr<Core> &core) {
    InitWindow(Config::window.width, Config::window.height, Config::title.data());
    core->load_assets();
    initialize_views(views, core);
    initialize_player(core);

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