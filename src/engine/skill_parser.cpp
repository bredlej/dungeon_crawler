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
    if (!json.contains(names[types::name].data())) {
        throw SkillParserException(fmt::format("Skill does not have a name: {}", json.dump(4)));
    }
}

static void validate_has_body_required(const nlohmann::json &json) {
    if (!json.contains(names[types::body_required].data())) {
        throw SkillParserException(fmt::format("Skill does not have a body_required: {}", json.dump(4)));
    }
}

static void validate_has_target_type(const nlohmann::json &json) {
    if (!json.contains(names[types::target_type].data())) {
        throw SkillParserException(fmt::format("Skill does not have a target_type: {}", json.dump(4)));
    }
}

static void validate_has_targets(const nlohmann::json &json) {
    if (!json.contains(names[types::targets].data())) {
        throw SkillParserException(fmt::format("Skill does not have a targets: {}", json.dump(4)));
    }
}

static void validate_skill_target_has_type(const nlohmann::json &json) {
    if (!json.contains(names[types::type].data())) {
        throw SkillParserException(fmt::format("Skill target does not have a type: {}", json.dump(4)));
    }
}

static void validate_skill_target_has_chance(const nlohmann::json &json) {
    if (!json.contains(names[types::chance].data())) {
        throw SkillParserException(fmt::format("Skill target does not have a chance: {}", json.dump(4)));
    }
}

static void validate_targets(const nlohmann::json &json) {
    if (!json[names[types::targets].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill targets is not an array: {}", json.dump(4)));
    }
    for (const auto& target: json[names[types::targets].data()]) {
        validate_skill_target_has_type(target);
        validate_skill_target_has_chance(target);
    }
}

static void validate_has_damage(const nlohmann::json &json) {
    if (!json.contains(names[types::damage].data())) {
        throw SkillParserException(fmt::format("Skill does not have a damage: {}", json.dump(4)));
    }
}

static void validate_ailment(const nlohmann::json &json) {
    if (!json[names[types::ailment].data()].is_string()) {
        throw SkillParserException(fmt::format("Skill damage ailment is not a string: {}", json.dump(4)));
    }
    if (!name_to_ailment.contains(json[names[types::ailment].data()].get<std::string_view>())) {
        throw SkillParserException(fmt::format("Skill damage ailment is not a valid ailment: {}", json.dump(4)));
    }
    if (!json[names[types::ailment].data()].contains(names[types::type].data())) {
        throw SkillParserException(fmt::format("Skill damage ailment does not have a type: {}", json.dump(4)));
    }
    if (name_to_ailment[json[names[types::ailment].data()][names[types::type].data()].get<std::string_view>()] == battle::Ailment::NONE) {
        throw SkillParserException(fmt::format("Skill damage ailment is not a valid ailment: {}", json.dump(4)));
    }
    if (!json[names[types::ailment].data()].contains(names[types::duration].data())) {
        throw SkillParserException(fmt::format("Skill damage ailment does not have a duration: {}", json.dump(4)));
    }
    if (!json[names[types::ailment].data()].contains(names[types::damage_value].data())) {
        throw SkillParserException(fmt::format("Skill damage ailment does not have a damage value: {}", json.dump(4)));
    }
    if (!json[names[types::ailment].data()].contains(names[types::chance].data())) {
        throw SkillParserException(fmt::format("Skill damage ailment does not have a chance: {}", json.dump(4)));
    }
}

static void validate_damage(const nlohmann::json &json) {
    if (!json[names[types::damage].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill damage is not an array: {}", json.dump(4)));
    }
    for (const auto& damage: json[names[types::damage].data()]) {
        if (damage.contains(names[types::ailment].data())) {
            validate_ailment(damage);
        }
        if (!damage.contains(names[types::duration].data())) {
            throw SkillParserException(fmt::format("Skill damage does not have a duration: {}", json.dump(4)));
        }
        if (!damage.contains(names[types::attribute].data())) {
            throw SkillParserException(fmt::format("Skill damage does not have an attribute: {}", json.dump(4)));
        }
        if (!damage.contains(names[types::damage_value].data())) {
            throw SkillParserException(fmt::format("Skill damage does not have a damage_value: {}", json.dump(4)));
        }
    }
}

static void validate_has_sp(const nlohmann::json &json) {
    if (!json.contains(names[types::sp].data())) {
        throw SkillParserException(fmt::format("Skill does not have a sp: {}", json.dump(4)));
    }
}

static void validate_has_hp(const nlohmann::json &json) {
    if (!json.contains(names[types::hp].data())) {
        throw SkillParserException(fmt::format("Skill does not have a hp: {}", json.dump(4)));
    }
}

static void validate_has_roles(const nlohmann::json &json) {
    if (!json.contains(names[types::roles].data())) {
        throw SkillParserException(fmt::format("Skill does not have a classes: {}", json.dump(4)));
    }
}

static void validate_roles(const nlohmann::json &json) {
    if (!json[names[types::roles].data()].is_array()) {
        throw SkillParserException(fmt::format("Skill classes is not an array: {}", json.dump(4)));
    }
    for (const auto& class_name: json[names[types::roles].data()]) {
        if (!class_name.is_string()) {
            throw SkillParserException(fmt::format("Skill class is not a string: {}", json.dump(4)));
        }
        if (!name_to_role.contains(class_name.get<std::string_view>())) {
            throw SkillParserException(fmt::format("Skill class is not a valid class: {}", json.dump(4)));
        }
    }
}

static void validate_offensive_skills(const nlohmann::json &json) {
    for (const auto& skill: json [names[types::offense].data()]) {
        validate_has_name(skill);
        validate_has_body_required(skill);
        validate_has_target_type(skill);
        validate_has_targets(skill);
        validate_targets(skill);
        validate_has_damage(skill);
        validate_damage(skill);
        validate_has_sp(skill);
        validate_has_hp(skill);
        validate_has_roles(skill);
        validate_roles(skill);
    }
}

void SkillParser::_validate(const nlohmann::json &json) {
    validate_offensive_skills(json);
}