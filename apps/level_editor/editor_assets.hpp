//
// Created by Patryk Szczypień on 10/04/2023.
//

#ifndef DUNGEON_CRAWLER_EDITOR_ASSETS_HPP
#define DUNGEON_CRAWLER_EDITOR_ASSETS_HPP
#include <asset_paths.hpp>
#include <assets.hpp>
#include <unordered_map>
#include <types.hpp>
#include <variant>
class EditorAssets {
public:
    EditorAssets() noexcept = default;
    EditorAssets(const EditorAssets&) noexcept = delete;
    EditorAssets(EditorAssets&&) noexcept = delete;
    EditorAssets& operator=(const EditorAssets&) noexcept = delete;
    EditorAssets& operator=(EditorAssets&&) noexcept = delete;
    void load_textures();
    std::unordered_map<std::variant<FloorType>, const assets::DCTexture> _textures;
};
#endif//DUNGEON_CRAWLER_EDITOR_ASSETS_HPP
