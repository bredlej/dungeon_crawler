//
// Created by Patryk Szczypień on 30/01/2024.
//
#include <monsters.hpp>
#include <parsers/monster_parser.hpp>
#include <parsers/attributes_parser.hpp>
MonstersMap MonstersMap::from_json(const nlohmann::json &json) {
    MonstersMap monsters_map;
    for (const auto &monster : json[monster_schema::names[monster_schema::schema_types::common].data()]) {
        auto id = monster[monster_schema::names[monster_schema::schema_types::id].data()].get<std::string>();
        const auto name = monster[monster_schema::names[monster_schema::schema_types::name].data()].get<std::string>();
        auto skills = monster[monster_schema::names[monster_schema::schema_types::skills].data()].get<std::vector<std::string>>();
        auto attributes = AttributesParser::parse_attributes(monster[monster_schema::names[monster_schema::schema_types::attributes].data()]);

        auto skill_ids = std::vector<components::general::Id>();
        for (const auto &skill : skills) {
            skill_ids.emplace_back(components::general::Id{skill});
        }

        monsters_map.monsters.emplace(id,  std::make_tuple(
            components::general::Id{id},
            components::general::Name{name},
            skill_ids,
            attributes
        ));
    }
    return monsters_map;
}