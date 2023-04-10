//
// Created by geoco on 17.11.2022.
//

#ifndef DUNGEON_CRAWLER_MAIN_MENU_HPP
#define DUNGEON_CRAWLER_MAIN_MENU_HPP

#include "engine/core.hpp"
#include <utility>
extern "C" {
#include <raylib.h>
};

class MainMenu : public UIView<MainMenu> {
public:
    explicit MainMenu(std::shared_ptr<Core> core) : UIView {core} {};
    void render() override;
    void update() override;
private:
};


#endif//DUNGEON_CRAWLER_MAIN_MENU_HPP
