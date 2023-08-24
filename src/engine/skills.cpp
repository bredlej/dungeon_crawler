//
// Created by Patryk Szczypień on 24/08/2023.
//
#include <skills.hpp>

using namespace skills;

static SkillsMap::OffensiveSkillMap map_offensive_skills(nlohmann::json &json) {
    using namespace skill_schema;
    SkillsMap::OffensiveSkillMap offensive_skills;
    for (auto &skill : json[names[schema_types::offense].data()]) {
        components::general::Name name{skill[names[schema_types::name].data()]};
        std::string body_part_string{skill[names[schema_types::body_required].data()]};
        types::battle::BodyPart body_part{types::battle::string_to_body_part[body_part_string]};
        std::string target_type_string{skill[names[schema_types::target_type].data()]};
        types::battle::TargetType target_type{types::battle::string_to_target_type[target_type_string]};
        SkillsMap::Targets targets;
        for (auto &skill_target : skill[names[schema_types::targets].data()]) {
            std::string target_string{skill_target[names[schema_types::type].data()]};
            types::battle::Target target{types::battle::string_to_target[target_string]};
            int chance{skill_target[names[schema_types::chance].data()]};
            targets.emplace_back(target, chance);
        }
        SkillsMap::Damage damage;
        for (auto &skill_damage : skill[names[schema_types::damage].data()]) {
            std::string type_string{skill_damage[names[schema_types::type].data()]};
            types::battle::AttackType type{types::battle::string_to_attack_type[type_string]};
            std::string attribute_string{skill_damage[names[schema_types::attribute].data()]};
            types::character::Attribute attribute{types::character::string_to_attribute[attribute_string]};
            int damage_value{skill_damage[names[schema_types::damage_value].data()]};
            damage.emplace_back(DamageDefinition{type, attribute, damage_value});
        }
        SkillsMap::Ailments ailments;
        for (auto &skill_ailment : skill[names[schema_types::ailments].data()]) {
            std::string type_string{skill_ailment[names[schema_types::type].data()]};
            types::battle::Ailment type{types::battle::string_to_ailment[type_string]};
            int chance{skill_ailment[names[schema_types::chance].data()]};
            int damage_value{skill_ailment[names[schema_types::damage_value].data()]};
            int duration{skill_ailment[names[schema_types::duration].data()]};
            ailments.emplace_back(AilmentDefinition{type, chance, damage_value, duration});
        }
        SkillCost skill_cost{skill[names[schema_types::sp].data()], skill[names[schema_types::hp].data()]};
        SkillsMap::RoleConstraints role_constraints;
        for (auto &skill_role : skill[names[schema_types::roles].data()]) {
            std::string role_string{skill_role};
            types::character::Role role{types::character::string_to_role[role_string]};
            role_constraints.emplace_back(role);
        }
        offensive_skills.emplace(name.name, std::make_tuple(name, body_part, target_type, targets, damage, ailments, skill_cost, role_constraints));
    }
    return offensive_skills;
}

SkillsMap skills::from_json(nlohmann::json &json) {
    return SkillsMap(
            map_offensive_skills(json));
}
