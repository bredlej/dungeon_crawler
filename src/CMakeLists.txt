set(INCLUDE_DIRS
        .
        ./ecs
        ./engine
        ./views
        ${PROJECT_SOURCE_DIR}/extern
        )
set(SOURCE_FILES
        application.cpp
        views/main_menu.cpp
        views/dungeon_view.cpp
        views/ui/dungeon_ui.cpp
        engine/core.cpp
        engine/combat.cpp
        engine/level_parser.cpp
        engine/skill_parser.cpp
        engine/level.cpp
        ecs/wall_map.cpp
        ecs/tile_map.cpp
        views/actions/dungeon_actions.cpp)

add_library(dungeon_crawler_application ${SOURCE_FILES})
target_link_libraries(dungeon_crawler_application raylib EnTT nlohmann_json::nlohmann_json imgui rlimgui fmt::fmt)
target_include_directories(dungeon_crawler_application PUBLIC ${INCLUDE_DIRS})
