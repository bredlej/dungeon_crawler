//
// Created by geoco on 17.11.2022.
//

#include "main_menu.hpp"
void MainMenu::render() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Main Menu", 10, 10, 42, WHITE);
    EndDrawing();
}
void MainMenu::update() {
}