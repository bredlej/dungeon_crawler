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
        DCTexture &operator=(const DCTexture other) noexcept { _texture = other._texture; };
        DCTexture &operator=(DCTexture &&other) noexcept { _texture = other._texture; };
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

    class Assets {
    public:
        explicit Assets() noexcept : _textures{} {
            _textures._tiles[dungeon_view::POVField::F01].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f01);
            _textures._tiles[dungeon_view::POVField::F02].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f02);
            _textures._tiles[dungeon_view::POVField::F03].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f03);
            _textures._tiles[dungeon_view::POVField::F04].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f04);
            _textures._tiles[dungeon_view::POVField::F05].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f05);
            _textures._tiles[dungeon_view::POVField::F06].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f06);
            _textures._tiles[dungeon_view::POVField::F07].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f07);
            _textures._tiles[dungeon_view::POVField::F08].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f08);
            _textures._tiles[dungeon_view::POVField::F09].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f09);
            _textures._tiles[dungeon_view::POVField::F10].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f10);
            _textures._tiles[dungeon_view::POVField::F11].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f11);
            _textures._tiles[dungeon_view::POVField::F12].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f12);
            _textures._tiles[dungeon_view::POVField::F13].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f13);
            _textures._tiles[dungeon_view::POVField::F14].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f14);
            _textures._tiles[dungeon_view::POVField::F15].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f15);
            _textures._tiles[dungeon_view::POVField::F16].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f16);
            _textures._tiles[dungeon_view::POVField::F17].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f17);
            _textures._tiles[dungeon_view::POVField::F18].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f18);
            _textures._tiles[dungeon_view::POVField::F19].emplace(FloorType::NORMAL, assets::tiles::floor::normal::f19);

            _textures._tiles[dungeon_view::POVField::F01].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c01);
            _textures._tiles[dungeon_view::POVField::F02].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c02);
            _textures._tiles[dungeon_view::POVField::F03].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c03);
            _textures._tiles[dungeon_view::POVField::F04].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c04);
            _textures._tiles[dungeon_view::POVField::F05].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c05);
            _textures._tiles[dungeon_view::POVField::F06].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c06);
            _textures._tiles[dungeon_view::POVField::F07].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c07);
            _textures._tiles[dungeon_view::POVField::F08].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c08);
            _textures._tiles[dungeon_view::POVField::F09].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c09);
            _textures._tiles[dungeon_view::POVField::F10].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c10);
            _textures._tiles[dungeon_view::POVField::F11].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c11);
            _textures._tiles[dungeon_view::POVField::F12].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c12);
            _textures._tiles[dungeon_view::POVField::F13].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c13);
            _textures._tiles[dungeon_view::POVField::F14].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c14);
            _textures._tiles[dungeon_view::POVField::F15].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c15);
            _textures._tiles[dungeon_view::POVField::F16].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c16);
            _textures._tiles[dungeon_view::POVField::F17].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c17);
            _textures._tiles[dungeon_view::POVField::F18].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c18);
            _textures._tiles[dungeon_view::POVField::F19].emplace(CeilingType::NORMAL, assets::tiles::ceiling::normal::c19);

            _textures._tiles[dungeon_view::POVField::F01].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall01);
            _textures._tiles[dungeon_view::POVField::F02].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall02);
            _textures._tiles[dungeon_view::POVField::F03].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall03);
            _textures._tiles[dungeon_view::POVField::F04].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall04);
            _textures._tiles[dungeon_view::POVField::F05].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall05);
            _textures._tiles[dungeon_view::POVField::F06].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall06);
            _textures._tiles[dungeon_view::POVField::F07].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall07);
            _textures._tiles[dungeon_view::POVField::F08].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall08);
            _textures._tiles[dungeon_view::POVField::F09].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall09);
            _textures._tiles[dungeon_view::POVField::F10].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall10);
            _textures._tiles[dungeon_view::POVField::F11].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall11);
            _textures._tiles[dungeon_view::POVField::F12].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall12);
            _textures._tiles[dungeon_view::POVField::F13].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall13);
            _textures._tiles[dungeon_view::POVField::F14].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall14);
            _textures._tiles[dungeon_view::POVField::F15].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall15);
            _textures._tiles[dungeon_view::POVField::F16].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall16);
            _textures._tiles[dungeon_view::POVField::F17].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall17);
            _textures._tiles[dungeon_view::POVField::F19].emplace(WallType::NORMAL, assets::tiles::wall::normal::wall19);
        };

        Textures _textures;
    };
}// namespace assets


#endif//DUNGEON_CRAWLER_ASSETS_HPP
