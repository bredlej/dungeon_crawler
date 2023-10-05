//
// Created by geoco on 22.11.2022.
//

#ifndef DUNGEON_CRAWLER_ASSETS_HPP
#define DUNGEON_CRAWLER_ASSETS_HPP
extern "C" {
#include <raylib.h>
};
#include <cstdint>
#include <cstdio>
#include <ecs/types.hpp>
#include <ecs/beasts.hpp>
#include <engine/asset_paths.hpp>
#include <unordered_map>
#include <variant>
#include <string>

using namespace types;
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
        DCTexture(DCTexture &&other) noexcept : _texture(other._texture) {};
        DCTexture &operator=(const DCTexture &other) noexcept { _texture = other._texture; return *this; };
        DCTexture &operator=(const DCTexture other) noexcept { _texture = other._texture; return *this; };
        DCTexture &operator=(DCTexture &&other) noexcept { _texture = other._texture; return *this; };
        ~DCTexture() = default;
        [[nodiscard]] Texture2D get() const {
            return _texture;
        }

    private:
        Texture2D _texture;
    };

    static std::unordered_map<std::string, FloorType> name_to_floor_type = {
            {"RUINS_01", FloorType::RUINS_01},
            {"RUINS_02", FloorType::RUINS_02}
    };
    static std::unordered_map<FloorType, std::string> floor_type_to_name = {
            {FloorType::RUINS_01, "RUINS_01"},
            {FloorType::RUINS_02, "RUINS_02"}
    };
    static std::unordered_map<std::string, CeilingType> name_to_ceiling_type = {
            {"RUINS_01", CeilingType::RUINS_01}
    };
    static std::unordered_map<CeilingType, std::string> ceiling_type_to_name = {
            {CeilingType::RUINS_01, "RUINS_01"}
    };
    static std::unordered_map<std::string, WallType> name_to_wall_type = {
            {"RUINS_01", WallType::RUINS_01}
    };
    static std::unordered_map<WallType, std::string> wall_type_to_name = {
            {WallType::RUINS_01, "RUINS_01"},
    };
    static std::unordered_map<DoorType, std::string> door_type_to_name = {
            {DoorType::RUINS_01_CLOSED, "RUINS_01_CLOSED"},
            {DoorType::RUINS_01_OPEN, "RUINS_01_OPEN"},
    };
    static std::unordered_map<std::string, DoorType> name_to_door_type = {
            {"RUINS_01_CLOSED", DoorType::RUINS_01_CLOSED},
            {"RUINS_01_OPEN", DoorType::RUINS_01_OPEN},
    };
    static std::unordered_map<DoorStateType, std::string> door_state_type_to_name = {
            {DoorStateType::OPEN, "Open"},
            {DoorStateType::CLOSED, "Closed"},
    };
    static std::unordered_map<std::string, DoorStateType> name_to_door_state_type = {
            {"Open", DoorStateType::OPEN},
            {"Closed", DoorStateType::CLOSED},
    };
    static std::unordered_map<std::string, Beast> name_to_beast_type = {
            {"GoblinWarrior", Beast::GoblinWarrior}
    };
    // clang-format off
    namespace dungeon_view {
        namespace GUI {
            enum class MiniMap : uint8_t {
                Background, Player
            };
        }
        enum class POVFloor : uint8_t {
            F01=0,F02,F03,F04,F05,//
            F06,F07,F08,F09,F10,//
            F11,F12,F13,//
            F14,F15,F16,//
            F17,F18,F19,//
            SIZE
        };
        enum class POVWall : uint8_t {
            W01_N=0, W01_E, W01_S, W02_N, W02_E, W02_S, W03_N, W03_S, W04_N, W04_S, W04_W, W05_N, W05_S, W05_W,//
            W06_E, W07_E, W07_S, W08_S, W09_S, W09_W, W10_W,//
            W11_E, W11_S, W12_S, W13_S, W13_W,//
            W14_E, W14_S, W15_S, W16_S, W16_W,//
            W17_E, W19_W,//
            SIZE
        };
    }
    // clang-format on

    using FieldMap = std::unordered_map<std::variant<FloorType, CeilingType, WallType, DoorType>, const assets::DCTexture>;
    struct Textures {
    public:
        explicit Textures() : _tiles(){};
        ~Textures() {
            for (const auto &[field, field_map]: _tiles) {
                for (const auto &[field_type, texture]: _tiles[field]) {
                    UnloadTexture(texture.get());
                }
            }
            for (const auto &[gui_element, texture]: _gui) {
                    UnloadTexture(texture.get());
            }
        }
        std::unordered_map<std::variant<dungeon_view::POVFloor, dungeon_view::POVWall>, FieldMap> _tiles;
        std::unordered_map<std::variant<dungeon_view::GUI::MiniMap>, const assets::DCTexture> _gui;
        std::unordered_map<Beast, const assets::DCTexture> _beasts;
        std::unordered_map<WorldDirection, const assets::DCTexture> _background;
    };

    struct Fonts {
    public:
        Fonts() : font{LoadFont("assets/Fonts/jupiterc.ttf")} {};
        ~Fonts() {UnloadFont(font);};
        Font font;
    };

    class Assets {
    public:
        explicit Assets() noexcept : _textures{} {
            _textures._background.emplace(WorldDirection::NORTH, DCTexture(assets::tiles::stratum::ruins::background::l01_north));
            _textures._background.emplace(WorldDirection::SOUTH, DCTexture(assets::tiles::stratum::ruins::background::l01_south));
            _textures._background.emplace(WorldDirection::EAST, DCTexture(assets::tiles::stratum::ruins::background::l01_east));
            _textures._background.emplace(WorldDirection::WEST, DCTexture(assets::tiles::stratum::ruins::background::l01_west));

            // ruins: floor type 1
            _textures._tiles[dungeon_view::POVFloor::F01].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f01);
            _textures._tiles[dungeon_view::POVFloor::F02].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f02);
            _textures._tiles[dungeon_view::POVFloor::F03].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f03);
            _textures._tiles[dungeon_view::POVFloor::F04].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f04);
            _textures._tiles[dungeon_view::POVFloor::F05].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f05);
            _textures._tiles[dungeon_view::POVFloor::F06].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f06);
            _textures._tiles[dungeon_view::POVFloor::F07].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f07);
            _textures._tiles[dungeon_view::POVFloor::F08].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f08);
            _textures._tiles[dungeon_view::POVFloor::F09].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f09);
            _textures._tiles[dungeon_view::POVFloor::F10].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f10);
            _textures._tiles[dungeon_view::POVFloor::F11].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f11);
            _textures._tiles[dungeon_view::POVFloor::F12].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f12);
            _textures._tiles[dungeon_view::POVFloor::F13].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f13);
            _textures._tiles[dungeon_view::POVFloor::F14].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f14);
            _textures._tiles[dungeon_view::POVFloor::F15].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f15);
            _textures._tiles[dungeon_view::POVFloor::F16].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f16);
            _textures._tiles[dungeon_view::POVFloor::F17].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f17);
            _textures._tiles[dungeon_view::POVFloor::F18].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f18);
            _textures._tiles[dungeon_view::POVFloor::F19].emplace(FloorType::RUINS_01, assets::tiles::stratum::ruins::floor::floor_01::f19);

            // ruins: floor type 2
            _textures._tiles[dungeon_view::POVFloor::F01].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f01);
            _textures._tiles[dungeon_view::POVFloor::F02].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f02);
            _textures._tiles[dungeon_view::POVFloor::F03].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f03);
            _textures._tiles[dungeon_view::POVFloor::F04].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f04);
            _textures._tiles[dungeon_view::POVFloor::F05].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f05);
            _textures._tiles[dungeon_view::POVFloor::F06].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f06);
            _textures._tiles[dungeon_view::POVFloor::F07].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f07);
            _textures._tiles[dungeon_view::POVFloor::F08].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f08);
            _textures._tiles[dungeon_view::POVFloor::F09].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f09);
            _textures._tiles[dungeon_view::POVFloor::F10].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f10);
            _textures._tiles[dungeon_view::POVFloor::F11].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f11);
            _textures._tiles[dungeon_view::POVFloor::F12].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f12);
            _textures._tiles[dungeon_view::POVFloor::F13].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f13);
            _textures._tiles[dungeon_view::POVFloor::F14].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f14);
            _textures._tiles[dungeon_view::POVFloor::F15].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f15);
            _textures._tiles[dungeon_view::POVFloor::F16].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f16);
            _textures._tiles[dungeon_view::POVFloor::F17].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f17);
            _textures._tiles[dungeon_view::POVFloor::F18].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f18);
            _textures._tiles[dungeon_view::POVFloor::F19].emplace(FloorType::RUINS_02, assets::tiles::stratum::ruins::floor::floor_02::f19);

            _textures._tiles[dungeon_view::POVFloor::F01].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c01);
            _textures._tiles[dungeon_view::POVFloor::F02].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c02);
            _textures._tiles[dungeon_view::POVFloor::F03].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c03);
            _textures._tiles[dungeon_view::POVFloor::F04].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c04);
            _textures._tiles[dungeon_view::POVFloor::F05].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c05);
            _textures._tiles[dungeon_view::POVFloor::F06].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c06);
            _textures._tiles[dungeon_view::POVFloor::F07].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c07);
            _textures._tiles[dungeon_view::POVFloor::F08].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c08);
            _textures._tiles[dungeon_view::POVFloor::F09].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c09);
            _textures._tiles[dungeon_view::POVFloor::F10].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c10);
            _textures._tiles[dungeon_view::POVFloor::F11].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c11);
            _textures._tiles[dungeon_view::POVFloor::F12].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c12);
            _textures._tiles[dungeon_view::POVFloor::F13].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c13);
            _textures._tiles[dungeon_view::POVFloor::F14].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c14);
            _textures._tiles[dungeon_view::POVFloor::F15].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c15);
            _textures._tiles[dungeon_view::POVFloor::F16].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c16);
            _textures._tiles[dungeon_view::POVFloor::F17].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c17);
            _textures._tiles[dungeon_view::POVFloor::F18].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c18);
            _textures._tiles[dungeon_view::POVFloor::F19].emplace(CeilingType::RUINS_01, assets::tiles::stratum::ruins::ceiling::ceiling_01::c19);

            _textures._tiles[dungeon_view::POVWall::W01_N].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w01_n);
            _textures._tiles[dungeon_view::POVWall::W01_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w01_e);
            _textures._tiles[dungeon_view::POVWall::W01_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w01_s);
            _textures._tiles[dungeon_view::POVWall::W02_N].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w02_n);
            _textures._tiles[dungeon_view::POVWall::W02_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w02_e);
            _textures._tiles[dungeon_view::POVWall::W02_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w02_s);
            _textures._tiles[dungeon_view::POVWall::W03_N].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w03_n);
            _textures._tiles[dungeon_view::POVWall::W03_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w03_s);
            _textures._tiles[dungeon_view::POVWall::W04_N].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w04_n);
            _textures._tiles[dungeon_view::POVWall::W04_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w04_w);
            _textures._tiles[dungeon_view::POVWall::W04_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w04_s);
            _textures._tiles[dungeon_view::POVWall::W05_N].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w05_n);
            _textures._tiles[dungeon_view::POVWall::W05_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w05_w);
            _textures._tiles[dungeon_view::POVWall::W05_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w05_s);
            _textures._tiles[dungeon_view::POVWall::W06_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w06_e);
            _textures._tiles[dungeon_view::POVWall::W07_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w07_e);
            _textures._tiles[dungeon_view::POVWall::W07_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w07_s);
            _textures._tiles[dungeon_view::POVWall::W08_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w08_s);
            _textures._tiles[dungeon_view::POVWall::W09_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w09_w);
            _textures._tiles[dungeon_view::POVWall::W09_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w09_s);
            _textures._tiles[dungeon_view::POVWall::W10_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w10_w);
            _textures._tiles[dungeon_view::POVWall::W11_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w11_e);
            _textures._tiles[dungeon_view::POVWall::W11_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w11_s);
            _textures._tiles[dungeon_view::POVWall::W12_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w12_s);
            _textures._tiles[dungeon_view::POVWall::W13_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w13_s);
            _textures._tiles[dungeon_view::POVWall::W13_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w13_w);
            _textures._tiles[dungeon_view::POVWall::W14_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w14_e);
            _textures._tiles[dungeon_view::POVWall::W14_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w14_s);
            _textures._tiles[dungeon_view::POVWall::W15_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w15_s);
            _textures._tiles[dungeon_view::POVWall::W16_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w16_w);
            _textures._tiles[dungeon_view::POVWall::W16_S].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w16_s);
            _textures._tiles[dungeon_view::POVWall::W17_E].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w17_e);
            _textures._tiles[dungeon_view::POVWall::W19_W].emplace(WallType::RUINS_01, assets::tiles::stratum::ruins::wall::wall_01::w19_w);

            // ruins door 01
            // closed
            _textures._tiles[dungeon_view::POVWall::W01_N].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d01c_n);
            _textures._tiles[dungeon_view::POVWall::W01_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d01c_e);
            _textures._tiles[dungeon_view::POVWall::W01_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d01c_s);
            _textures._tiles[dungeon_view::POVWall::W02_N].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d02c_n);
            _textures._tiles[dungeon_view::POVWall::W02_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d02c_e);
            _textures._tiles[dungeon_view::POVWall::W02_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d02c_s);
            _textures._tiles[dungeon_view::POVWall::W03_N].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d03c_n);
            _textures._tiles[dungeon_view::POVWall::W03_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d03c_s);
            _textures._tiles[dungeon_view::POVWall::W04_N].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d04c_n);
            _textures._tiles[dungeon_view::POVWall::W04_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d04c_w);
            _textures._tiles[dungeon_view::POVWall::W04_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d04c_s);
            _textures._tiles[dungeon_view::POVWall::W05_N].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d05c_n);
            _textures._tiles[dungeon_view::POVWall::W05_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d05c_w);
            _textures._tiles[dungeon_view::POVWall::W05_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d05c_s);
            _textures._tiles[dungeon_view::POVWall::W06_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d06c_e);
            _textures._tiles[dungeon_view::POVWall::W07_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d07c_e);
            _textures._tiles[dungeon_view::POVWall::W07_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d07c_s);
            _textures._tiles[dungeon_view::POVWall::W08_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d08c_s);
            _textures._tiles[dungeon_view::POVWall::W09_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d09c_w);
            _textures._tiles[dungeon_view::POVWall::W09_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d09c_s);
            _textures._tiles[dungeon_view::POVWall::W10_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d10c_w);
            _textures._tiles[dungeon_view::POVWall::W11_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d11c_e);
            _textures._tiles[dungeon_view::POVWall::W11_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d11c_s);
            _textures._tiles[dungeon_view::POVWall::W12_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d12c_s);
            _textures._tiles[dungeon_view::POVWall::W13_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d13c_w);
            _textures._tiles[dungeon_view::POVWall::W13_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d13c_s);
            _textures._tiles[dungeon_view::POVWall::W14_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d14c_e);
            _textures._tiles[dungeon_view::POVWall::W14_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d14c_s);
            _textures._tiles[dungeon_view::POVWall::W15_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d15c_s);
            _textures._tiles[dungeon_view::POVWall::W16_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d16c_w);
            _textures._tiles[dungeon_view::POVWall::W16_S].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d16c_s);
            _textures._tiles[dungeon_view::POVWall::W17_E].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d17c_e);
            _textures._tiles[dungeon_view::POVWall::W19_W].emplace(DoorType::RUINS_01_CLOSED, assets::tiles::stratum::ruins::door_01::d19c_w);
            // open
            _textures._tiles[dungeon_view::POVWall::W01_N].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d01o_n);
            _textures._tiles[dungeon_view::POVWall::W01_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d01o_e);
            _textures._tiles[dungeon_view::POVWall::W01_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d01o_s);
            _textures._tiles[dungeon_view::POVWall::W02_N].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d02o_n);
            _textures._tiles[dungeon_view::POVWall::W02_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d02o_e);
            _textures._tiles[dungeon_view::POVWall::W02_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d02o_s);
            _textures._tiles[dungeon_view::POVWall::W03_N].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d03o_n);
            _textures._tiles[dungeon_view::POVWall::W03_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d03o_s);
            _textures._tiles[dungeon_view::POVWall::W04_N].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d04o_n);
            _textures._tiles[dungeon_view::POVWall::W04_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d04o_w);
            _textures._tiles[dungeon_view::POVWall::W04_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d04o_s);
            _textures._tiles[dungeon_view::POVWall::W05_N].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d05o_n);
            _textures._tiles[dungeon_view::POVWall::W05_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d05o_w);
            _textures._tiles[dungeon_view::POVWall::W05_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d05o_s);
            _textures._tiles[dungeon_view::POVWall::W06_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d06o_e);
            _textures._tiles[dungeon_view::POVWall::W07_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d07o_e);
            _textures._tiles[dungeon_view::POVWall::W07_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d07o_s);
            _textures._tiles[dungeon_view::POVWall::W08_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d08o_s);
            _textures._tiles[dungeon_view::POVWall::W09_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d09o_w);
            _textures._tiles[dungeon_view::POVWall::W09_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d09o_s);
            _textures._tiles[dungeon_view::POVWall::W10_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d10o_w);
            _textures._tiles[dungeon_view::POVWall::W11_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d11o_e);
            _textures._tiles[dungeon_view::POVWall::W11_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d11o_s);
            _textures._tiles[dungeon_view::POVWall::W12_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d12o_s);
            _textures._tiles[dungeon_view::POVWall::W13_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d13o_w);
            _textures._tiles[dungeon_view::POVWall::W13_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d13o_s);
            _textures._tiles[dungeon_view::POVWall::W14_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d14o_e);
            _textures._tiles[dungeon_view::POVWall::W14_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d14o_s);
            _textures._tiles[dungeon_view::POVWall::W15_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d15o_s);
            _textures._tiles[dungeon_view::POVWall::W16_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d16o_w);
            _textures._tiles[dungeon_view::POVWall::W16_S].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d16o_s);
            _textures._tiles[dungeon_view::POVWall::W17_E].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d17o_e);
            _textures._tiles[dungeon_view::POVWall::W19_W].emplace(DoorType::RUINS_01_OPEN, assets::tiles::stratum::ruins::door_01::d19o_w);

            // GUI
            _textures._gui.emplace(dungeon_view::GUI::MiniMap::Background, assets::gui::minimap::background);
            _textures._gui.emplace(dungeon_view::GUI::MiniMap::Player, assets::gui::minimap::player_ss);

            // Bestiary
            _textures._beasts.emplace(Beast::GoblinWarrior, assets::monsters::goblin_warrior);
        };

        Textures _textures;
        Fonts fonts;
    };
}// namespace assets


#endif//DUNGEON_CRAWLER_ASSETS_HPP
