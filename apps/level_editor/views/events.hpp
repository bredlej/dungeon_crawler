//
// Created by Patryk Szczypień on 05/04/2023.
//

#ifndef DUNGEON_CRAWLER_EVENTS_HPP
#define DUNGEON_CRAWLER_EVENTS_HPP
enum class EditMode {
    None, Tile, Wall
};
struct CurrentEditMode {
    EditMode edit_mode;
};
struct ChangeEditMode {EditMode edit_mode;};
#endif//DUNGEON_CRAWLER_EVENTS_HPP
