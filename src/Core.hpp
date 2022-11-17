//
// Created by geoco on 17.11.2022.
//

#ifndef DUNGEON_CRAWLER_CORE_HPP
#define DUNGEON_CRAWLER_CORE_HPP
struct UIView {
    virtual void render() = 0;
    virtual void update() = 0;
    virtual ~UIView() = default;
};
#endif//DUNGEON_CRAWLER_CORE_HPP
