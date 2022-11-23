//
// Created by geoco on 22.11.2022.
//

#ifndef DUNGEON_CRAWLER_ASSETS_HPP
#define DUNGEON_CRAWLER_ASSETS_HPP
extern "C" {
#include <raylib.h>
};
#include "asset_paths.hpp"
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <variant>
namespace assets {
    struct DCTexture {
    public:
        explicit DCTexture(const char *file) {
            auto image = LoadImage(file);
            _texture = LoadTextureFromImage(image);
            UnloadImage(image);
        };
        explicit DCTexture() = default;
        DCTexture(const Texture &other) noexcept = delete;
        DCTexture(DCTexture &other) noexcept = delete;
        DCTexture(DCTexture &&other) noexcept : _texture(other._texture){};
        DCTexture &operator=(const DCTexture &other) noexcept { _texture = other._texture; };
        ~DCTexture() = default;
        [[nodiscard]] Texture2D get() const {
            return _texture;
        }

    private:
        Texture2D _texture;
    };


    enum class FloorType {
        NORMAL
    };
    enum class CeilingType {
        NORMAL
    };
    enum class WallType {
        NORMAL
    };

    // clang-format off
    namespace dungeon_view {
        enum class POVField : uint8_t {
            F01,F02,F03,F04,F05,F06,F07,F08,F09,F10,F11,F12,F13,F14,F15,F16,F17,F18,F19//
        };
    }
    // clang-format on

    using FieldMap = std::unordered_map<std::variant<FloorType, CeilingType, WallType>, const assets::DCTexture>;
    struct Textures {
    public:
        explicit Textures() : _tiles(){};
        ~Textures() {
            for (const auto &[field, field_map]: _tiles) {
                for (const auto &[field_type, texture]: _tiles[field]) {
                    UnloadTexture(texture.get());
                }
            }
        }
        std::unordered_map<dungeon_view::POVField, FieldMap> _tiles;
    };

    inline static FieldMap get_field_map(std::variant<FloorType, CeilingType, WallType> field_type, const char *path) {
        FieldMap f;
        f.emplace(std::make_pair(field_type, std::move(DCTexture{path})));
        return f;
    };

    class Assets {
    public:
        explicit Assets() noexcept : _textures{} {
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F01, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f01)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F02, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f02)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F03, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f03)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F04, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f04)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F05, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f05)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F06, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f06)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F07, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f07)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F08, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f08)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F09, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f09)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F10, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f10)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F11, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f11)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F12, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f12)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F13, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f13)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F14, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f14)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F15, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f15)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F16, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f16)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F17, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f17)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F18, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f18)));
            _textures._tiles.emplace(std::make_pair(dungeon_view::POVField::F19, get_field_map(FloorType::NORMAL, assets::tiles::floor::normal::f19)));
        };

        Textures _textures;
    };
}// namespace assets


#endif//DUNGEON_CRAWLER_ASSETS_HPP
