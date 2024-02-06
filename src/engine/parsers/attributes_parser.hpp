//
// Created by Patryk Szczypień on 29/01/2024.
//

#ifndef ATTRIBUTES_PARSER_HPP
#define ATTRIBUTES_PARSER_HPP
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <ecs/components.hpp>
#include <ecs/types.hpp>

namespace attributes_schema {
    enum class schema_types {
        attributes,
        hit_points,
        skill_points,
        strength,
        intelligence,
        vitality,
        wisdom,
        dexterity,
        luck,
        attack,
        magic_attack,
        defense,
        magic_defense,
    };
    static std::unordered_map<schema_types, std::string_view> names {
        {schema_types::attributes, "attributes"},
        {schema_types::hit_points, "HIT_POINTS"},
        {schema_types::skill_points, "SKILL_POINTS"},
        {schema_types::strength, "STRENGTH"},
        {schema_types::intelligence, "INTELLIGENCE"},
        {schema_types::vitality, "VITALITY"},
        {schema_types::wisdom, "WISDOM"},
        {schema_types::dexterity, "DEXTERITY"},
        {schema_types::luck, "LUCK"},
        {schema_types::attack, "ATTACK"},
        {schema_types::magic_attack, "MAGIC_ATTACK"},
        {schema_types::defense, "DEFENSE"},
        {schema_types::magic_defense, "MAGIC_DEFENSE"}
    };
}

class AttributesParser {
public:
    static nlohmann::json parse_json(const nlohmann::json &json);
    static components::battle::Attributes parse_attributes(const nlohmann::json &json);
private:
    static void _validate(const nlohmann::json &json);
};
#endif //ATTRIBUTES_PARSER_HPP
