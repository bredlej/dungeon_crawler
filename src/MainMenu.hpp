//
// Created by geoco on 17.11.2022.
//

#ifndef DUNGEON_CRAWLER_MAINMENU_HPP
#define DUNGEON_CRAWLER_MAINMENU_HPP

#include <Core.hpp>
extern "C" {
#include <raylib.h>
};

class MainMenu : public UIView {
    void render() override;
    void update() override;
};


#endif//DUNGEON_CRAWLER_MAINMENU_HPP
