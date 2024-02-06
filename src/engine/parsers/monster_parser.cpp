//
// Created by Patryk Szczypień on 26/01/2024.
//
#include <parsers/monster_parser.hpp>

nlohmann::json MonsterParser::parse(const std::string &path, const AttributesParser &attributes_parser, const skills::SkillsMap &skills_map) {
    std::ifstream file(path.c_str());
    auto json = nlohmann::json::parse(file);
    _validate(json, skills_map);
    return json;
}

void MonsterParser::save(const std::string &path, const nlohmann::json &json) {
    std::ofstream file(path.c_str());
    file << json.dump(4);
}

static void validate_has_id(const nlohmann::json &json) {
    if (!json.contains(monster_schema::names[monster_schema::schema_types::id].data())) {
        throw MonsterParserException(fmt::format("Monster does not have an id: {}", json.dump(4)));
    }
}

static void validate_has_name(const nlohmann::json &json) {
    if (!json.contains(monster_schema::names[monster_schema::schema_types::name].data())) {
        throw MonsterParserException(fmt::format("Monster does not have a name: {}", json.dump(4)));
    }
}

static void validate_has_attributes(const nlohmann::json &json) {
    if (!json.contains(monster_schema::names[monster_schema::schema_types::attributes].data())) {
        throw MonsterParserException(fmt::format("Monster does not have attributes: {}", json.dump(4)));
    }
}

static void validate_has_skills(const nlohmann::json &json) {
    if (!json.contains(monster_schema::names[monster_schema::schema_types::skills].data())) {
        throw MonsterParserException(fmt::format("Monster does not have skills: {}", json.dump(4)));
    }
}
static void validate_skills_exist(const nlohmann::json &json, const skills::SkillsMap &skills_map) {
        for (const auto &skill : json[monster_schema::names[monster_schema::schema_types::skills].data()]) {
            if (!skills_map.monster_skills.contains(skill.get<std::string>())) {
                throw MonsterParserException(fmt::format("Monster has a skill that does not exist: {}", skill.get<std::string>()));
            }
        }
}
void MonsterParser::_validate(const nlohmann::json &json, const skills::SkillsMap &skills_map) {
    if (!json.contains(monster_schema::names[monster_schema::schema_types::common].data())) {
        throw MonsterParserException(fmt::format("Monster does not have a common section: {}", json.dump(4)));
    }

    for (const auto common = json[monster_schema::names[monster_schema::schema_types::common].data()];
        const auto& monster : common) {
        validate_has_id(monster);
        validate_has_name(monster);
        validate_has_attributes(monster);
        validate_has_skills(monster);
        validate_skills_exist(monster, skills_map);
    }

}