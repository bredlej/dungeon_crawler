set(INCLUDE_DIRS
        .
        ${PROJECT_SOURCE_DIR}/extern
        )
set(SOURCE_FILES
        Application.cpp
        MainMenu.cpp)

add_library(dungeon_crawler_application ${SOURCE_FILES})
target_link_libraries(dungeon_crawler_application raylib)
target_include_directories(dungeon_crawler_application PRIVATE ${INCLUDE_DIRS})