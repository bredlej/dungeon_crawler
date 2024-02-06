#include <skills.hpp>
//
// Created by Patryk Szczypień on 24/08/2023.
//

using namespace skills;
using namespace skill_schema;

static SkillsMap::Targets map_targets(const nlohmann::json &skill) {
    SkillsMap::Targets targets;
    for (auto &skill_target: skill[names[schema_types::targets].data()]) {
        std::string target_string{skill_target[names[schema_types::type].data()]};
        types::battle::Target target{types::battle::string_to_target[target_string]};
        int chance{skill_target[names[schema_types::chance].data()]};
        targets.emplace_back(target, chance);
    }
    return targets;
}
static SkillsMap::Damage map_damage(const nlohmann::json &damage) {
    SkillsMap::Damage damage_definitions;
    for (auto &skill_damage: damage[names[schema_types::damage].data()]) {
        std::string type_string{skill_damage[names[schema_types::type].data()]};
        types::battle::AttackType type{types::battle::string_to_attack_type[type_string]};
        std::string attribute_string{skill_damage[names[schema_types::attribute].data()]};
        types::character::Attribute attribute{types::character::string_to_attribute[attribute_string]};
        int damage_value{skill_damage[names[schema_types::damage_value].data()]};
        damage_definitions.emplace_back(DamageDefinition{type, attribute, static_cast<float>(damage_value)});
    }
    return damage_definitions;
}
static SkillsMap::Ailments map_ailments(const nlohmann::json &ailments) {
    SkillsMap::Ailments ailment_definitions;
    for (auto &skill_ailment: ailments[names[schema_types::ailments].data()]) {
        std::string type_string{skill_ailment[names[schema_types::type].data()]};
        types::battle::Ailment type{types::battle::string_to_ailment[type_string]};
        int chance{skill_ailment[names[schema_types::chance].data()]};
        int damage_value{skill_ailment[names[schema_types::damage_value].data()]};
        int duration{skill_ailment[names[schema_types::duration].data()]};
        ailment_definitions.emplace_back(AilmentDefinition{type, chance, damage_value, duration});
    }
    return ailment_definitions;
}
static SkillsMap::RoleConstraints map_role_constraints(const nlohmann::json &roles) {
    SkillsMap::RoleConstraints role_constraints;
    for (auto &skill_role: roles[names[schema_types::roles].data()]) {
        std::string role_string{skill_role};
        types::character::Role role{types::character::string_to_role[role_string]};
        role_constraints.emplace_back(role);
    }
    return role_constraints;
}

SkillsMap::Followups map_followups(const nlohmann::json &json) {
    SkillsMap::Followups followup_definitions;
    for (auto &json_followup: json[names[schema_types::followups].data()]) {
        std::vector<types::battle::AttackType> on_damage_type;
        for (auto &damage_type: json_followup[names[schema_types::on_damage_type].data()]) {
            std::string type_string{damage_type};
            types::battle::AttackType type{types::battle::string_to_attack_type[type_string]};
            on_damage_type.emplace_back(type);
        }
        int duration{json_followup[names[schema_types::duration].data()]};
        int max_stack{json_followup[names[schema_types::max_stack].data()]};
        int initial_chance{json_followup[names[schema_types::initial_chance].data()]};
        int damage_reduction_percent{json_followup[names[schema_types::damage_reduction_percent].data()]};

        followup_definitions.emplace_back(FollowupDefinition{
                on_damage_type,
                duration,
                max_stack,
                initial_chance,
                damage_reduction_percent,
                map_damage(json_followup),
                map_ailments(json_followup)});
    }
    return followup_definitions;
}

static components::general::SkillCost map_skill_cost(const nlohmann::json &json) {
    return components::general::SkillCost{json[names[schema_types::sp].data()], json[names[schema_types::hp].data()]};
}
static components::general::Id map_id(const nlohmann::json &json) {
    return components::general::Id{json[names[schema_types::id].data()]};
}
static components::general::Name map_name(const nlohmann::json &json) {
    return components::general::Name{json[names[schema_types::name].data()]};
}
static types::battle::BodyPart map_body_part(const nlohmann::json &json) {
    return types::battle::string_to_body_part[std::string{json[names[schema_types::body_required].data()]}];
}
static types::battle::TargetType map_target_type(const nlohmann::json &json) {
    return types::battle::string_to_target_type[std::string{json[names[schema_types::target_type].data()]}];
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

template<typename DAMAGE>
static void emplace_damage(const std::shared_ptr<Core> &core, entt::entity skill, const auto &damage_definition) {
    core->registry.emplace<DAMAGE>(skill, static_cast<float>(damage_definition.damage_value));
}

static void map_damage(const std::shared_ptr<Core> &core, entt::entity skill, const SkillsMap::Damage &damage) {
    for (auto &damage_definition: damage) {
        switch (damage_definition.type) {
            case battle::AttackType::BASH:
                break;
            case battle::AttackType::CUT:
                break;
            case battle::AttackType::PIERCE:
                emplace_damage<components::battle::damage_types::PierceDmg>(core, skill, damage_definition);
                break;
            case battle::AttackType::FIRE:
                emplace_damage<components::battle::damage_types::FireDmg>(core, skill, damage_definition);
                break;
            case battle::AttackType::ICE:
                emplace_damage<components::battle::damage_types::IceDmg>(core, skill, damage_definition);
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

template<typename AILMENT>
static void emplace_ailment(const std::shared_ptr<Core> &core, entt::entity skill, const auto &ailment_definition) {
    core->registry.emplace<AILMENT>(skill,
                                    static_cast<float>(ailment_definition.damage_value),
                                    static_cast<float>(ailment_definition.chance),
                                    static_cast<uint32_t>(ailment_definition.duration));
}
static void map_ailments(const std::shared_ptr<Core> &core, entt::entity skill, const SkillsMap::Ailments &ailments) {
    for (auto &ailment_definition: ailments) {
        switch (ailment_definition.type) {
            case battle::Ailment::NONE:
                break;
            case battle::Ailment::BURN:
                emplace_ailment<components::battle::ailments::Burn>(core, skill, ailment_definition);
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
static void map_id(const std::shared_ptr<Core> &core, entt::entity skill, const components::general::Id &id) {
    core->registry.emplace<components::general::Id>(skill, id.id);
}
static void map_name(const std::shared_ptr<Core> &core, entt::entity skill, const components::general::Name &name) {
    core->registry.emplace<components::general::Name>(skill, name.name);
}
static void map_followup_attacks(const std::shared_ptr<Core> &core, entt::entity skill, const skills::SkillsMap::Followups &followups) {
    for (auto &followup: followups) {
        auto followup_attack = components::battle::FollowupAttack{entt::null, skill, followup.duration, followup.max_stack, followup.initial_chance, followup.damage_reduction_percent};
        for (auto &damage: followup.damage) {
            auto followup_damage = core->registry.create();
            switch (damage.type) {
                case battle::AttackType::BASH:
                    break;
                case battle::AttackType::CUT:
                    break;
                case battle::AttackType::PIERCE:
                    emplace_damage<components::battle::damage_types::PierceDmg>(core, followup_damage, damage);
                    break;
                case battle::AttackType::FIRE:
                    emplace_damage<components::battle::damage_types::FireDmg>(core, followup_damage, damage);
                    break;
                case battle::AttackType::ICE:
                    emplace_damage<components::battle::damage_types::IceDmg>(core, followup_damage, damage);
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
            followup_attack.damage.emplace_back(followup_damage);
        }
        core->registry.emplace_or_replace<components::battle::FollowupAttack>(skill, followup_attack);
    }
}

std::function<entt::entity(const std::shared_ptr<Core> &)> SkillsMap::instance_offensive_skill(const std::shared_ptr<Core> &core, const SkillsMap &skills_map, const std::string &skill_name) {
    return [&](const std::shared_ptr<Core> &c) -> entt::entity {
        entt::entity e = entt::null;
        if (skills_map.offensive_skills.contains(skill_name)) {
            e = core->registry.create();
            const auto skill = skills_map.offensive_skills.at(skill_name);
            map_id(core, e, std::get<components::general::Id>(skill));
            map_name(core, e, std::get<components::general::Name>(skill));
            map_body_constraints(core, e, std::get<types::battle::BodyPart>(skill));
            map_targets(core, e, std::get<skills::SkillsMap::Targets>(skill));
            map_damage(core, e, std::get<skills::SkillsMap::Damage>(skill));
            map_ailments(core, e, std::get<skills::SkillsMap::Ailments>(skill));
            map_skill_cost(core, e, std::get<components::general::SkillCost>(skill));
            map_followup_attacks(core, e, std::get<skills::SkillsMap::Followups>(skill));
        }

        return e;
    };
}

static SkillsMap::OffensiveSkillMap map_offensive_skills(const nlohmann::json &json) {

    SkillsMap::OffensiveSkillMap offensive_skills;
    for (auto &skill: json[names[schema_types::offense].data()]) {
        components::general::Id id = map_id(skill);
        components::general::Name name = map_name(skill);

        offensive_skills.emplace(id.id,
                                 std::make_tuple(id, name,
                                                 map_body_part(skill),
                                                 map_target_type(skill),
                                                 map_targets(skill),
                                                 map_damage(skill),
                                                 map_ailments(skill),
                                                 map_skill_cost(skill),
                                                 map_role_constraints(skill),
                                                 map_followups(skill)));
    }
    return offensive_skills;
}

static SkillsMap::MonsterSkillMap map_monster_skills(const nlohmann::json &json) {
    SkillsMap::MonsterSkillMap monster_skills;

    for (auto &skill: json[names[schema_types::monster_skills].data()]) {
components::general::Id id = map_id(skill);
        components::general::Name name = map_name(skill);

        monster_skills.emplace(id.id,
                               std::make_tuple(id, name,
                                               map_body_part(skill),
                                               map_target_type(skill),
                                               map_targets(skill),
                                               map_damage(skill),
                                               map_ailments(skill),
                                               map_skill_cost(skill),
                                               map_followups(skill)));
    }
    return monster_skills;
}

SkillsMap SkillsMap::from_json(const nlohmann::json &json) {
    return SkillsMap(
            map_offensive_skills(json), map_monster_skills(json));
}
