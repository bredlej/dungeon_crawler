# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.
option(BUILD_LEVEL_EDITOR "Build the level editor" ON)
if (BUILD_LEVEL_EDITOR)
set(EDITOR_SOURCE_FILES
        level_editor.cpp
        editor_application.cpp
        editor_assets.cpp
        views/main_view.cpp
        views/level_editor_tab/map_view.cpp
        views/level_editor_tab/components/main_menu.cpp
        views/level_editor_tab/components/edit_mode.cpp
        views/level_editor_tab/components/file_operations.cpp
        views/level_editor_tab/components/level_list.cpp
        views/level_editor_tab/components/entity_details.cpp
        views/level_editor_tab/components/blueprint.cpp
        views/level_editor_tab/components/component_renderer.cpp
        views/battle_editor_tab/battle_view.cpp
)
set(EDITOR_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/apps/level_editor
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views/level_editor_tab
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views/battle_editor_tab
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views/level_editor_tab/components
        ${INCLUDE_DIR})

add_executable(level_editor ${EDITOR_SOURCE_FILES})
endif ()