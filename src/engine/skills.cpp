//
// Created by Patryk Szczypień on 24/08/2023.
//
#include <skills.hpp>

using namespace skills;

static SkillsMap::OffensiveSkillMap map_offensive_skills(nlohmann::json &json) {
    using namespace skill_schema;
    SkillsMap::OffensiveSkillMap offensive_skills;
    for (auto &skill: json[names[schema_types::offense].data()]) {
        components::general::Name name{skill[names[schema_types::name].data()]};
        std::string body_part_string{skill[names[schema_types::body_required].data()]};
        types::battle::BodyPart body_part{types::battle::string_to_body_part[body_part_string]};
        std::string target_type_string{skill[names[schema_types::target_type].data()]};
        types::battle::TargetType target_type{types::battle::string_to_target_type[target_type_string]};
        SkillsMap::Targets targets;
        for (auto &skill_target: skill[names[schema_types::targets].data()]) {
            std::string target_string{skill_target[names[schema_types::type].data()]};
            types::battle::Target target{types::battle::string_to_target[target_string]};
            int chance{skill_target[names[schema_types::chance].data()]};
            targets.emplace_back(target, chance);
        }
        SkillsMap::Damage damage;
        for (auto &skill_damage: skill[names[schema_types::damage].data()]) {
            std::string type_string{skill_damage[names[schema_types::type].data()]};
            types::battle::AttackType type{types::battle::string_to_attack_type[type_string]};
            std::string attribute_string{skill_damage[names[schema_types::attribute].data()]};
            types::character::Attribute attribute{types::character::string_to_attribute[attribute_string]};
            int damage_value{skill_damage[names[schema_types::damage_value].data()]};
            damage.emplace_back(DamageDefinition{type, attribute, static_cast<float>(damage_value)});
        }
        SkillsMap::Ailments ailments;
        for (auto &skill_ailment: skill[names[schema_types::ailments].data()]) {
            std::string type_string{skill_ailment[names[schema_types::type].data()]};
            types::battle::Ailment type{types::battle::string_to_ailment[type_string]};
            int chance{skill_ailment[names[schema_types::chance].data()]};
            int damage_value{skill_ailment[names[schema_types::damage_value].data()]};
            int duration{skill_ailment[names[schema_types::duration].data()]};
            ailments.emplace_back(AilmentDefinition{type, chance, damage_value, duration});
        }
        components::general::SkillCost skill_cost{skill[names[schema_types::sp].data()], skill[names[schema_types::hp].data()]};
        SkillsMap::RoleConstraints role_constraints;
        for (auto &skill_role: skill[names[schema_types::roles].data()]) {
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

static void map_targets(const std::shared_ptr<Core> &core, entt::entity skill, const SkillsMap::Targets &targets) {
    for (auto &target: targets) {
        switch (target.first) {
            case battle::Target::ALL:
                break;
            case battle::Target::SELF:
                break;
            case battle::Target::SINGLE:
                core->registry.emplace<components::battle::targets::TargetSingle>(skill, entt::null, static_cast<float>(target.second));
                break;
            case battle::Target::NEIGHBOURS:
                core->registry.emplace<components::battle::targets::TargetMultiple>(skill, std::vector<entt::entity>(), static_cast<float>(target.second));
                break;
            case battle::Target::HORIZONTAL:
                break;
            case battle::Target::VERTICAL:
                break;
            case battle::Target::SINGLE_WITH_NEIGHBOURS:
                break;
        }
    }
}

static void map_damage(const std::shared_ptr<Core> &core, entt::entity skill, const SkillsMap::Damage &damage) {
    for (auto &damage_definition: damage) {
        switch (damage_definition.type) {
            case battle::AttackType::BASH:
                break;
            case battle::AttackType::CUT:
                break;
            case battle::AttackType::PIERCE:
                break;
            case battle::AttackType::FIRE:
                core->registry.emplace<components::battle::damage_types::FireDmg>(skill, static_cast<float>(damage_definition.damage_value));
                break;
            case battle::AttackType::ICE:
                break;
            case battle::AttackType::LIGHTNING:
                break;
            case battle::AttackType::POISON:
                break;
            case battle::AttackType::DARKNESS:
                break;
            case battle::AttackType::HOLY:
                break;
        }
    }
}

static void map_ailments(const std::shared_ptr<Core> &core, entt::entity skill, const SkillsMap::Ailments &ailments) {
    for (auto &ailment_definition: ailments) {
        switch (ailment_definition.type) {
            case battle::Ailment::NONE:
                break;
            case battle::Ailment::BURN:
                core->registry.emplace<components::battle::ailments::Burn>(skill,
                                                                           static_cast<float>(ailment_definition.damage_value),
                                                                           static_cast<float>(ailment_definition.chance),
                                                                           static_cast<uint32_t>(ailment_definition.duration));
                break;
            case battle::Ailment::FREEZE:
            case battle::Ailment::PARALYSIS:
            case battle::Ailment::POISON:
            case battle::Ailment::SLEEP:
            case battle::Ailment::STUN:
            case battle::Ailment::CONFUSE:
            case battle::Ailment::SILENCE:
            case battle::Ailment::BLIND:
            case battle::Ailment::CHARM:
            case battle::Ailment::BLEED:
            case battle::Ailment::PETRIFICATION:
            case battle::Ailment::DOOM:
            case battle::Ailment::FEAR:
            case battle::Ailment::CURSE:
            case battle::Ailment::WEAKNESS:
            case battle::Ailment::SLOW:
                break;
            case battle::Ailment::DEATH:
                core->registry.emplace<components::battle::ailments::Death>(skill);
                break;
        }
    }
}
static void map_skill_cost(const std::shared_ptr<Core> &core, entt::entity skill, const components::general::SkillCost &skill_cost) {
    core->registry.emplace<components::general::SkillCost>(skill, skill_cost.sp, skill_cost.hp);
}
static void map_body_constraints(const std::shared_ptr<Core> &core, entt::entity skill, const types::battle::BodyPart &body_part) {
    core->registry.emplace<components::battle::BodyConstraints>(skill, std::vector<types::battle::BodyPart>{body_part});
}
static void map_name(const std::shared_ptr<Core> &core, entt::entity skill, const components::general::Name &name) {
    core->registry.emplace<components::general::Name>(skill, name.name);
}

std::function<entt::entity(const std::shared_ptr<Core> &)> skills::instance_offensive_skill(const std::shared_ptr<Core> &core, const SkillsMap &skills_map, const std::string &skill_name) {
    return [&](const std::shared_ptr<Core> &c) -> entt::entity {
        entt::entity e = entt::null;
        if (skills_map.offensive_skills.contains(skill_name)) {
            e = core->registry.create();
            auto skill = skills_map.offensive_skills.at(skill_name);

            map_name(core, e, std::get<components::general::Name>(skill));
            map_body_constraints(core, e, std::get<types::battle::BodyPart>(skill));
            map_targets(core, e, std::get<skills::SkillsMap::Targets>(skill));
            map_damage(core, e, std::get<skills::SkillsMap::Damage>(skill));
            map_ailments(core, e, std::get<skills::SkillsMap::Ailments>(skill));
            map_skill_cost(core, e, std::get<components::general::SkillCost>(skill));
        }

        return e;
    };
}
