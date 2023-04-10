//
// Created by Patryk Szczypień on 10/04/2023.
//

#ifndef DUNGEON_CRAWLER_EDIT_MODE_HPP
#define DUNGEON_CRAWLER_EDIT_MODE_HPP
#include <core.hpp>
#include <memory>
#include <events.hpp>

class EditModeSelector {
public:
    explicit EditModeSelector(std::shared_ptr<Core> core) : _core{core} {};
    EditModeSelector(const EditModeSelector &) = delete;
    EditModeSelector(EditModeSelector &&) = default;
    EditModeSelector &operator=(const EditModeSelector &) = delete;
    EditModeSelector &operator=(EditModeSelector &&) = delete;
    ~EditModeSelector() = default;
    void render();
    void update();
private:
    bool _visible{true};
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_EDIT_MODE_HPP
