# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.
set(EDITOR_SOURCE_FILES
        level_editor.cpp
        editor_application.cpp
        editor_assets.cpp
        ./views/main_view.cpp
        ./views/map_view.cpp
        ./views/components/main_menu.cpp
        ./views/components/edit_mode.cpp
        ./views/components/file_operations.cpp
        ./views/components/level_list.cpp
        ./views/components/entity_details.cpp
        ./views/components/blueprint.cpp
        ./views/components/component_renderer.cpp)
set(EDITOR_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/apps/level_editor
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views
        ${PROJECT_SOURCE_DIR}/apps/level_editor/views/components
        ${INCLUDE_DIR})

add_executable(level_editor ${EDITOR_SOURCE_FILES})
if (${WASM})
target_link_libraries(level_editor dungeon_crawler_application glfw3)
else ()
target_link_libraries(level_editor dungeon_crawler_application)
endif ()
target_include_directories(level_editor PRIVATE ${EDITOR_INCLUDE_DIRS})
if (EMSCRIPTEN AND ${WASM})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 -s USE_GLFW=3 -s TOTAL_MEMORY=64MB -s ASSERTIONS=1 -s WASM=1 -s  ASYNCIFY ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 -s USE_GLFW=3 --preload-file assets -s TOTAL_MEMORY=32MB -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY ")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
    add_custom_command(TARGET level_editor
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ../docs/generated)
    add_custom_command(TARGET level_editor
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy level_editor.js ../docs/generated/level_editor.js)
    add_custom_command(TARGET level_editor
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy level_editor.wasm ../docs/generated/level_editor.wasm)
    add_custom_command(TARGET level_editor
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy level_editor.html ../docs/generated/level_editor.html)
    add_custom_command(TARGET level_editor
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy level_editor.data ../docs/generated/level_editor.data)
endif ()
