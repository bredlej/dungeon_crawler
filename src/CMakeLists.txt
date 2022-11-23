set(INCLUDE_DIRS
        .
        ${PROJECT_SOURCE_DIR}/extern
        )
set(SOURCE_FILES
        application.cpp
        views/main_menu.cpp views/dungeon_view.cpp views/dungeon_view.hpp engine/assets.hpp engine/asset_paths.hpp engine/core.cpp engine/textures.hpp)

add_library(dungeon_crawler_application ${SOURCE_FILES})
target_link_libraries(dungeon_crawler_application raylib EnTT)
target_include_directories(dungeon_crawler_application PRIVATE ${INCLUDE_DIRS})
