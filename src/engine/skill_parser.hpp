//
// Created by Patryk Szczypień on 21/08/2023.
//

#ifndef DUNGEON_CRAWLER_SKILL_PARSER_HPP
#define DUNGEON_CRAWLER_SKILL_PARSER_HPP
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <fmt/format.h>
#include <ecs/types.hpp>

class SkillParserException : public std::exception {
    const std::string _message;;
    
public:
    explicit SkillParserException(const std::string message) : _message(message) {}
    explicit SkillParserException() = delete;
    explicit SkillParserException(SkillParserException &&) = delete;
    [[nodiscard]] const char * what() const noexcept override {
        return _message.c_str();
    }
};

namespace skill_schema {
    enum class types {
        offense,
        name,
        body_required,
        target_type,
        targets,
        type,
        chance,
        damage,
        ailment,
        duration,
        attribute,
        damage_value,
        sp,
        hp,
        roles
    };
    static std::unordered_map<types, std::string_view> names {
            {types::offense, "offense"},
            {types::name, "name"},
            {types::body_required, "body_required"},
            {types::target_type, "target_type"},
            {types::targets, "targets"},
            {types::type, "type"},
            {types::chance, "chance"},
            {types::damage, "damage"},
            {types::ailment, "ailment"},
            {types::duration, "duration"},
            {types::attribute, "attribute"},
            {types::damage_value, "damage_value"},
            {types::sp, "sp"},
            {types::hp, "hp"},
            {types::roles, "roles"}
    };

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
            {"NONE", battle::Ailment::NONE}
    };

    static std::unordered_map<std::string_view, character::Role> name_to_role = {
            {"CULTIST", character::Role::CULTIST},
            {"HEAVY_KNIGHT", character::Role::HEAVY_KNIGHT},
            {"MAGE", character::Role::MAGE},
            {"THIEF", character::Role::THIEF},
            {"WARRIOR", character::Role::WARRIOR}
    };
}

class SkillParser {
public:
    static nlohmann::json parse(const std::string &path);
    static nlohmann::json parse_json(const nlohmann::json &json);
    static void save(const std::string &path, const nlohmann::json &json);
private:
    static void _validate(const nlohmann::json &json);
};


#endif//DUNGEON_CRAWLER_SKILL_PARSER_HPP
