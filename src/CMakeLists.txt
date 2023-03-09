set(INCLUDE_DIRS
        .
        ./ecs
        ./engine
        ./views
        ${PROJECT_SOURCE_DIR}/extern
        )
set(SOURCE_FILES
        application.cpp
        views/main_menu.cpp views/dungeon_view.cpp engine/core.cpp ecs/wall_map.cpp ecs/tile_map.cpp engine/level_parser.cpp)

add_library(dungeon_crawler_application ${SOURCE_FILES})
target_link_libraries(dungeon_crawler_application raylib EnTT nlohmann_json::nlohmann_json)
target_include_directories(dungeon_crawler_application PRIVATE ${INCLUDE_DIRS})
