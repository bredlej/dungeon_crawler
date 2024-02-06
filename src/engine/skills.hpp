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
#include <parsers/skill_parser.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>

namespace skills {
    struct DamageDefinition {
        battle::AttackType type;
        character::Attribute attribute;
        float damage_value;
    };

    struct AilmentDefinition {
        battle::Ailment type;
        int chance;
        int damage_value;
        int duration;
    };

    struct FollowupDefinition {
        std::vector<battle::AttackType> on_damage_type;
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

    class SkillsMap {
    public:
        using Targets = std::vector<std::pair<types::battle::Target, int>>;
        using Damage = std::vector<DamageDefinition>;
        using Ailments = std::vector<AilmentDefinition>;
        using RoleConstraints = std::vector<types::character::Role>;
        using Followups = std::vector<FollowupDefinition>;
        using OffensiveSkill = std::tuple<
                components::general::Id,
                components::general::Name,
                types::battle::BodyPart,
                types::battle::TargetType,
                Targets,
                Damage,
                Ailments,
                components::general::SkillCost,
                RoleConstraints,
                Followups>;
        using MonsterSkill = std::tuple<
                components::general::Id,
                components::general::Name,
                types::battle::BodyPart,
                types::battle::TargetType,
                Targets,
                Damage,
                Ailments,
                components::general::SkillCost,
                Followups>;
        using OffensiveSkillMap = std::unordered_map<std::string, OffensiveSkill>;
        using MonsterSkillMap = std::unordered_map<std::string, MonsterSkill>;

        explicit SkillsMap(OffensiveSkillMap &&offensive_skill_map, MonsterSkillMap &&monster_skill_map) : offensive_skills(std::move(offensive_skill_map)), monster_skills(std::move(monster_skill_map)){};
        SkillsMap(const SkillsMap &) = default;
        SkillsMap(SkillsMap &&) = default;
        SkillsMap &operator=(const SkillsMap &) = delete;
        SkillsMap &operator=(SkillsMap &&) = delete;
        ~SkillsMap() = default;

        static SkillsMap from_json(const nlohmann::json &json);
        static std::function<entt::entity(const std::shared_ptr<Core> &)> instance_offensive_skill(const std::shared_ptr<Core> &core, const SkillsMap &skills_map, const std::string &skill_name);

        OffensiveSkillMap offensive_skills;
        MonsterSkillMap monster_skills;


    };


}// namespace skills

#endif//DUNGEON_CRAWLER_SKILLS_HPP
