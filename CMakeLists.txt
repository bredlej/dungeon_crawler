cmake_minimum_required(VERSION 3.23)
project(dungeon_crawler)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
include(CTest)
include(cmake/CPM.cmake)
include(cmake/fetch-json.cmake)
include(cmake/fetch-googletest.cmake)
include(cmake/fetch-entt.cmake)
add_subdirectory(extern)
add_subdirectory(src)
add_executable(dungeon_crawler main.cpp)
target_link_libraries(dungeon_crawler dungeon_crawler_application)
target_include_directories(dungeon_crawler PRIVATE src/)

if(APPLE)
    find_library(GLFW glfw3 PATHS glfw)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    set(EXTRA_LIBS
            ${COCOA_LIBRARY}
            ${OPENGL_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
            ${GLFW}
            )
endif(APPLE)
if (DEFINED ENV{BUILD_WASM})
    message("-- Building for WebAssembly")
    set(WASM ON)
else ()
    message("-- Building for Desktop")
    set(WASM OFF)
endif ()

enable_testing()

set(IMGUI_SOURCES
        extern/imgui/imgui.cpp
        extern/imgui/imgui_demo.cpp
        extern/imgui/imgui_draw.cpp
        extern/imgui/imgui_widgets.cpp
        extern/imgui/imgui_tables.cpp
        extern/imgui/imgui.h
        extern/imgui/imgui_internal.h
        extern/imgui/misc/cpp/imgui_stdlib.cpp
        extern/imgui/misc/cpp/imgui_stdlib.h)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC extern/imgui)

add_library(rlimgui extern/rlimgui/rlImGui.cpp extern/rlimgui/rlImGui.h)
target_link_libraries(rlimgui PUBLIC imgui raylib)
target_include_directories(rlimgui PUBLIC raylib/src/external/glfw/include)