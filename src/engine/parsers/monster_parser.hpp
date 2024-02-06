//
// Created by Patryk Szczypień on 26/01/2024.
//

#ifndef MONSTER_PARSER_HPP
#define MONSTER_PARSER_HPP
#include <nlohmann/json.hpp>
#include <ecs/types.hpp>
#include <skills.hpp>
#include <engine/parsers/attributes_parser.hpp>

using namespace types;


class MonsterParserException final : public std::exception {
    const std::string _message;
    ;

public:
    explicit MonsterParserException(const std::string &message) : _message(message) {}
    explicit MonsterParserException() = delete;
    explicit MonsterParserException(MonsterParserException &&) = delete;
    [[nodiscard]] const char *what() const noexcept override {
        return _message.c_str();
    }
};

namespace monster_schema {
    enum class schema_types {
        common,
        id,
        name,
        attributes,
        skills,
    };

    static std::unordered_map<schema_types, std::string_view> names {
        {schema_types::common, "common"},
        {schema_types::id, "id"},
        {schema_types::name, "name"},
        {schema_types::attributes, "attributes"},
        {schema_types::skills, "skills"},
    };
}

class MonsterParser {
public:
    static nlohmann::json parse(const std::string &path, const AttributesParser &attributes_parser, const skills::SkillsMap &skills_map);
    static void save(const std::string &path, const nlohmann::json &json);

private:
    static void _validate(const nlohmann::json &json, const skills::SkillsMap &skills_map);
};

#endif //MONSTER_PARSER_HPP
