//
// Created by Patryk Szczypień on 23/03/2023.
//

#ifndef DUNGEON_CRAWLER_COLORS_HPP
#define DUNGEON_CRAWLER_COLORS_HPP
#include <raylib.h>
#include <array>
namespace palette {
    constexpr static Color col_0 = {0x8f, 0xbf, 0xd5, 0xff};
    constexpr static Color light_blue = col_0;
    constexpr static Color col_1 = {0x83, 0x85, 0xcf, 0xff};
    constexpr static Color light_violet = col_1;
    constexpr static Color col_2 = {0x76, 0x55, 0xa2, 0xff};
    constexpr static Color violet = col_2;
    constexpr static Color col_3 = {0x31, 0x3a, 0x91, 0xff};
    constexpr static Color blue = col_3;
    constexpr static Color col_4 = {0xcd, 0x93, 0x73, 0xff};
    constexpr static Color skin = col_4;
    constexpr static Color col_5 = {0xb1, 0x48, 0x63, 0xff};
    constexpr static Color rose = col_5;
    constexpr static Color col_6 = {0x91, 0x31, 0x31, 0xff};
    constexpr static Color red = col_6;
    constexpr static Color col_7 = {0xd5, 0xbb, 0x4f, 0xff};
    constexpr static Color yellow = col_7;
    constexpr static Color col_8 = {0xa7, 0x74, 0x34, 0xff};
    constexpr static Color light_brown = col_8;
    constexpr static Color col_9 = {0x78, 0x42, 0x33, 0xff};
    constexpr static Color brown = col_9;
    constexpr static Color col_10 = {0x53, 0x3b, 0x40, 0xff};
    constexpr static Color dark_brown = col_10;
    constexpr static Color col_11 = {0x34, 0x31, 0x1d, 0xff};
    constexpr static Color brown_green = col_11;
    constexpr static Color col_12 = {0x66, 0xac, 0x7d, 0xff};
    constexpr static Color lime = col_12;
    constexpr static Color col_13 = {0x4a, 0x78, 0x33, 0xff};
    constexpr static Color green = col_13;
    constexpr static Color col_14 = {0x48, 0x53, 0x22, 0xff};
    constexpr static Color dark_green = col_14;
    constexpr static Color col_15 = {0x7a, 0x7d, 0x5d, 0xff};
    constexpr static Color olive = col_15;
    constexpr static Color col_16 = {0x5e, 0x51, 0x38, 0xff};
    constexpr static Color green_brown = col_16;
    constexpr static Color col_17 = {0x49, 0x35, 0x2b, 0xff};
    constexpr static Color mud_brown = col_17;
    constexpr static Color col_18 = {0xed, 0xe6, 0xc8, 0xff};
    constexpr static Color whiteish = col_18;
    constexpr static Color col_19 = {0x80, 0x80, 0x78, 0xff};
    constexpr static Color gray = col_19;
    constexpr static Color col_20 = {0x48, 0x54, 0x54, 0xff};
    constexpr static Color blueish_gray = col_20;
    constexpr static Color col_21 = {0x37, 0x3a, 0x3e, 0xff};
    constexpr static Color dark_gray = col_21;
    constexpr static Color col_22 = {0x27, 0x18, 0x18, 0xff};
    constexpr static Color reddish_black = col_22;

    constexpr static std::array<Color, 17> all{col_0, col_1, col_2, col_3, col_4, col_5, col_6, col_7, col_8, col_9, col_10, col_11, col_12, col_13, col_14, col_15, col_16};
}
#endif//DUNGEON_CRAWLER_COLORS_HPP
