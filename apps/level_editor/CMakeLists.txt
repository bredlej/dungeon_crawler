# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.
set(EDITOR_SOURCE_FILES
        level_editor.cpp
        editor_application.cpp
        ./views/main_view.cpp
        ./views/map_view.cpp
        ./views/components/main_menu.cpp)
set(EDITOR_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/apps/level_editor
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views/components
        ${INCLUDE_DIR})

add_executable(level_editor ${EDITOR_SOURCE_FILES})
target_link_libraries(level_editor dungeon_crawler_application)
target_include_directories(level_editor PRIVATE ${EDITOR_INCLUDE_DIRS})
