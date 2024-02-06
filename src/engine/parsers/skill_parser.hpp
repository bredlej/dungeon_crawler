//
// Created by Patryk Szczypień on 21/08/2023.
//

#ifndef DUNGEON_CRAWLER_SKILL_PARSER_HPP
#define DUNGEON_CRAWLER_SKILL_PARSER_HPP
#include <ecs/types.hpp>
#include <fmt/format.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using namespace types;

class SkillParserException final : public std::exception {
    const std::string _message;
    ;

public:
    explicit SkillParserException(const std::string &message) : _message(message) {}
    explicit SkillParserException() = delete;
    explicit SkillParserException(SkillParserException &&) = delete;
    [[nodiscard]] const char *what() const noexcept override {
        return _message.c_str();
    }
};

namespace skill_schema {
    enum class schema_types {
        offense,
        monster_skills,
        id,
        name,
        body_required,
        target_type,
        targets,
        type,
        chance,
        damage,
        ailments,
        duration,
        attribute,
        damage_value,
        sp,
        hp,
        roles,
        followups,
        on_damage_type,
        max_stack,
        initial_chance,
        damage_reduction_percent
    };
    static std::unordered_map<schema_types, std::string_view> names{
            {schema_types::offense, "offense"},
            {schema_types::monster_skills, "monster_skills"},
            {schema_types::id, "id"},
            {schema_types::name, "name"},
            {schema_types::body_required, "body_required"},
            {schema_types::target_type, "target_type"},
            {schema_types::targets, "targets"},
            {schema_types::type, "type"},
            {schema_types::chance, "chance"},
            {schema_types::damage, "damage"},
            {schema_types::ailments, "ailments"},
            {schema_types::duration, "duration"},
            {schema_types::attribute, "attribute"},
            {schema_types::damage_value, "damage_value"},
            {schema_types::sp, "sp"},
            {schema_types::hp, "hp"},
            {schema_types::roles, "roles"},
            {schema_types::followups, "followups"},
            {schema_types::on_damage_type, "on_damage_type"},
            {schema_types::max_stack, "max_stack"},
            {schema_types::initial_chance, "initial_chance"},
            {schema_types::damage_reduction_percent, "damage_reduction_percent"}};

    static std::unordered_map<std::string_view, battle::Ailment> name_to_ailment = {
            {"BURN", battle::Ailment::BURN},
            {"POISON", battle::Ailment::POISON},
            {"PARALYSIS", battle::Ailment::PARALYSIS},
            {"SLEEP", battle::Ailment::SLEEP},
            {"FREEZE", battle::Ailment::FREEZE},
            {"CONFUSE", battle::Ailment::CONFUSE},
            {"BLIND", battle::Ailment::BLIND},
            {"SILENCE", battle::Ailment::SILENCE},
            {"BLEED", battle::Ailment::BLEED},
            {"STUN", battle::Ailment::STUN},
            {"CURSE", battle::Ailment::CURSE},
            {"DEATH", battle::Ailment::DEATH},
            {"NONE", battle::Ailment::NONE}};

    static std::unordered_map<std::string_view, character::Role> name_to_role = {
            {"CULTIST", character::Role::CULTIST},
            {"HEAVY_KNIGHT", character::Role::HEAVY_KNIGHT},
            {"MAGE", character::Role::MAGE},
            {"THIEF", character::Role::THIEF},
            {"WARRIOR", character::Role::WARRIOR},
            {"BLADE_DANCER", character::Role::BLADE_DANCER}};
}// namespace skill_schema

class SkillParser {
public:
    static nlohmann::json parse(const std::string &path);
    static nlohmann::json parse_json(const nlohmann::json &json);
    static void save(const std::string &path, const nlohmann::json &json);

private:
    static void _validate(const nlohmann::json &json);
};


#endif//DUNGEON_CRAWLER_SKILL_PARSER_HPP
