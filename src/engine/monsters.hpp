//
// Created by Patryk Szczypień on 30/01/2024.
//

#ifndef MONSTERS_HPP
#define MONSTERS_HPP
#include <ecs/components.hpp>
#include <ecs/types.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

class MonstersMap {
public:
    using Monster = std::tuple<
        components::general::Id,
        components::general::Name,
        std::vector<components::general::Id>, // skills
        components::battle::Attributes>;

    std::unordered_map<std::string, Monster> monsters;

    static MonstersMap from_json(const nlohmann::json &json);
};
#endif //MONSTERS_HPP
