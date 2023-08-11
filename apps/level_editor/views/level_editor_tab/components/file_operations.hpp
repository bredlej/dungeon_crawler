//
// Created by Patryk Szczypień on 10/04/2023.
//

#ifndef DUNGEON_CRAWLER_FILE_OPERATIONS_HPP
#define DUNGEON_CRAWLER_FILE_OPERATIONS_HPP
#include <events.hpp>
#include <core.hpp>
#include <memory>
class FileOperations {
public:
    explicit FileOperations(std::shared_ptr<Core> core) : _core{core} {
        _core->dispatcher.sink<editor::LoadLevel>().connect<&FileOperations::_change_level_name>(this);
    };
    FileOperations(const FileOperations &) = delete;
    FileOperations(FileOperations &&) = default;
    FileOperations &operator=(const FileOperations &) = delete;
    FileOperations &operator=(FileOperations &&) = delete;
    ~FileOperations() = default;
    void render();
    void update();

private:
    void _change_level_name(const editor::LoadLevel &load_level);
    char _save_as[256] = "level_1.json";
    bool _visible{true};
    std::shared_ptr<Core> _core;
};
#endif//DUNGEON_CRAWLER_FILE_OPERATIONS_HPP
