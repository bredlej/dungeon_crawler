//
// Created by Patryk Szczypień on 21/08/2023.
//

#include "skill_parser.hpp"

using namespace skill_schema;

nlohmann::json SkillParser::parse(const std::string &path) {
    std::ifstream file(path.c_str());
    auto json = nlohmann::json::parse(file);
    _validate(json);
    return json;
}

nlohmann::json SkillParser::parse_json(const nlohmann::json &json) {
    _validate(json);
    return json;
}

void SkillParser::save(const std::string &path, const nlohmann::json &json) {
    std::ofstream file(path.c_str());
    file << json.dump(4);
}

static void validate_has_name(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::name].data())) {
        throw SkillParserException(fmt::format("Skill does not have a name: {}", json.dump(4)));
    }
}

static void validate_has_body_required(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::body_required].data())) {
        throw SkillParserException(fmt::format("Skill does not have a body_required: {}", json.dump(4)));
    }
}

static void validate_has_target_type(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::target_type].data())) {
        throw SkillParserException(fmt::format("Skill does not have a target_type: {}", json.dump(4)));
    }
}

static void validate_has_targets(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::targets].data())) {
        throw SkillParserException(fmt::format("Skill does not have a targets: {}", json.dump(4)));
    }
}

static void validate_skill_target_has_type(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::type].data())) {
        throw SkillParserException(fmt::format("Skill target does not have a type: {}", json.dump(4)));
    }
}

static void validate_skill_target_has_chance(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::chance].data())) {
        throw SkillParserException(fmt::format("Skill target does not have a chance: {}", json.dump(4)));
    }
}

static void validate_targets(const nlohmann::json &json) {
    if (!json[names[schema_types::targets].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill targets is not an array: {}", json.dump(4)));
    }
    for (const auto& target: json[names[schema_types::targets].data()]) {
        validate_skill_target_has_type(target);
        validate_skill_target_has_chance(target);
    }
}

static void validate_has_damage(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::damage].data())) {
        throw SkillParserException(fmt::format("Skill does not have a damage: {}", json.dump(4)));
    }
}

static void validate_has_ailments(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::ailments].data())) {
        throw SkillParserException(fmt::format("Skill does not have a ailments: {}", json.dump(4)));
    }
}
static void validate_ailments(const nlohmann::json &json) {
    if (!json[names[schema_types::ailments].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill ailments is not an array: {}", json.dump(4)));
    }
    for (const auto& ailment: json[names[schema_types::ailments].data()]) {
        if (!ailment.contains(names[schema_types::duration].data())) {
            throw SkillParserException(fmt::format("Skill ailment does not have a duration: {}", json.dump(4)));
        }
        if (!ailment.contains(names[schema_types::type].data())) {
            throw SkillParserException(fmt::format("Skill ailment does not have a type: {}", json.dump(4)));
        }
        if (!ailment.contains(names[schema_types::damage_value].data())) {
            throw SkillParserException(fmt::format("Skill ailment does not have a damage_value: {}", json.dump(4)));
        }
        if (!ailment.contains(names[schema_types::chance].data())) {
            throw SkillParserException(fmt::format("Skill ailment does not have a chance: {}", json.dump(4)));
        }
    }
}

static void validate_damage(const nlohmann::json &json) {
    if (!json[names[schema_types::damage].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill damage is not an array: {}", json.dump(4)));
    }
    for (const auto& damage: json[names[schema_types::damage].data()]) {
        if (!damage.contains(names[schema_types::type].data())) {
            throw SkillParserException(fmt::format("Skill damage does not have a type: {}", json.dump(4)));
        }
        if (!damage.contains(names[schema_types::attribute].data())) {
            throw SkillParserException(fmt::format("Skill damage does not have an attribute: {}", json.dump(4)));
        }
        if (!damage.contains(names[schema_types::damage_value].data())) {
            throw SkillParserException(fmt::format("Skill damage does not have a damage_value: {}", json.dump(4)));
        }
    }
}

static void validate_has_sp(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::sp].data())) {
        throw SkillParserException(fmt::format("Skill does not have a sp: {}", json.dump(4)));
    }
}

static void validate_has_hp(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::hp].data())) {
        throw SkillParserException(fmt::format("Skill does not have a hp: {}", json.dump(4)));
    }
}

static void validate_has_roles(const nlohmann::json &json) {
    if (!json.contains(names[schema_types::roles].data())) {
        throw SkillParserException(fmt::format("Skill does not have a classes: {}", json.dump(4)));
    }
}

static void validate_roles(const nlohmann::json &json) {
    if (!json[names[schema_types::roles].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill classes is not an array: {}", json.dump(4)));
    }
    for (const auto& class_name: json[names[schema_types::roles].data()]) {
        if (!class_name.is_string()) {
            throw SkillParserException(fmt::format("Skill class is not a string: {}", json.dump(4)));
        }
        if (!name_to_role.contains(class_name.get<std::string_view>())) {
            throw SkillParserException(fmt::format("Skill class is not a valid class: {}", json.dump(4)));
        }
    }
}

static void validate_offensive_skills(const nlohmann::json &json) {
    if (json.contains(names[schema_types::offense].data()) && !json[names[schema_types::offense].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill offense is not an array: {}", json.dump(4)));
    }
    if (json.contains(names[schema_types::offense].data())) {
        for (const auto& skill: json [names[schema_types::offense].data()]) {
            validate_has_name(skill);
            validate_has_body_required(skill);
            validate_has_target_type(skill);
            validate_has_targets(skill);
            validate_targets(skill);
            validate_has_damage(skill);
            validate_damage(skill);
            validate_has_ailments(skill);
            validate_ailments(skill);
            validate_has_sp(skill);
            validate_has_hp(skill);
            validate_has_roles(skill);
            validate_roles(skill);
        }
    }
}

void SkillParser::_validate(const nlohmann::json &json) {
    validate_offensive_skills(json);
}