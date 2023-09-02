//
// Created by Patryk Szczypień on 23/08/2023.
//

#ifndef DUNGEON_CRAWLER_SKILLS_HPP
#define DUNGEON_CRAWLER_SKILLS_HPP
#include <core.hpp>
#include <ecs/components.hpp>
#include <ecs/types.hpp>
#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <skill_parser.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>

namespace skills {
    struct DamageDefinition {
        types::battle::AttackType type;
        types::character::Attribute attribute;
        float damage_value;
    };

    struct AilmentDefinition {
        types::battle::Ailment type;
        int chance;
        int damage_value;
        int duration;
    };

    struct FollowupDefinition {
        std::vector<types::battle::AttackType> on_damage_type;
        int duration;
        int max_stack;
        int initial_chance;
        int damage_reduction_percent;
        std::vector<DamageDefinition> damage;
        std::vector<AilmentDefinition> ailments;
    };

    enum class UseSkillResult {
        SUCCESS,
        NOT_ENOUGH_HP,
        NOT_ENOUGH_SP,
        WRONG_TARGETS,
        WRONG_ROLE,
        UNAVAILABLE_BODY_PART,
        WRONG_TARGET_TYPE,
        FORBIDDEN_BY_AILMENT,
        UNAVAILABLE_SKILL
    };

    struct SkillsMap {
    public:
        using Targets = std::vector<std::pair<types::battle::Target, int>>;
        using Damage = std::vector<DamageDefinition>;
        using Ailments = std::vector<AilmentDefinition>;
        using RoleConstraints = std::vector<types::character::Role>;
        using Followups = std::vector<FollowupDefinition>;
        using OffensiveSkill = std::tuple<
                components::general::Name,
                types::battle::BodyPart,
                types::battle::TargetType,
                Targets,
                Damage,
                Ailments,
                components::general::SkillCost,
                RoleConstraints,
                Followups>;
        using OffensiveSkillMap = std::unordered_map<std::string, OffensiveSkill>;

        explicit SkillsMap(OffensiveSkillMap &&offensive_skill_map) : offensive_skills(std::move(offensive_skill_map)){};

        SkillsMap(const SkillsMap &) = delete;
        SkillsMap(SkillsMap &&) = delete;
        SkillsMap &operator=(const SkillsMap &) = delete;
        SkillsMap &operator=(SkillsMap &&) = delete;
        ~SkillsMap() = default;

        OffensiveSkillMap offensive_skills;
    };

    SkillsMap from_json(nlohmann::json &json);

    std::function<entt::entity(const std::shared_ptr<Core> &)> instance_offensive_skill(const std::shared_ptr<Core> &core, const SkillsMap &skills_map, const std::string &skill_name);
}// namespace skills

#endif//DUNGEON_CRAWLER_SKILLS_HPP
