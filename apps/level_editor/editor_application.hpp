//
// Created by Patryk Szczypień on 03/04/2023.
//

#ifndef DUNGEON_CRAWLER_EDITOR_APPLICATION_HPP
#define DUNGEON_CRAWLER_EDITOR_APPLICATION_HPP
#include <cstdint>
extern "C" {
#include <raylib.h>
}
#include <main_view.hpp>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>

class EditorApplication {
public:
    explicit EditorApplication() noexcept = default;
    EditorApplication(const EditorApplication&) noexcept = delete;
    EditorApplication(EditorApplication&&) noexcept = delete;
    EditorApplication& operator=(const EditorApplication&) noexcept = delete;
    EditorApplication& operator=(EditorApplication&&) noexcept = delete;
    void run() const;
private:
    uint32_t _window_width{1024};
    uint32_t _window_height{768};
};
#endif//DUNGEON_CRAWLER_EDITOR_APPLICATION_HPP