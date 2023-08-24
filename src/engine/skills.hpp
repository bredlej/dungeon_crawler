//
// Created by Patryk Szczypień on 23/08/2023.
//

#ifndef DUNGEON_CRAWLER_SKILLS_HPP
#define DUNGEON_CRAWLER_SKILLS_HPP
#include <ecs/components.hpp>
#include <ecs/types.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <nlohmann/json.hpp>
#include <skill_parser.hpp>

namespace skills {
    struct DamageDefinition {
        types::battle::AttackType type;
        types::character::Attribute attribute;
        int damage_value;
    };

    struct AilmentDefinition {
        types::battle::Ailment type;
        int chance;
        int damage_value;
        int duration;
    };

    struct SkillCost {
        int sp;
        int hp;
    };

    struct SkillsMap {
    public:
        using Targets = std::vector<std::pair<types::battle::Target, int>>;
        using Damage = std::vector<DamageDefinition>;
        using Ailments = std::vector<AilmentDefinition>;
        using RoleConstraints = std::vector<types::character::Role>;
        using OffensiveSkill = std::tuple<
                components::general::Name,
                types::battle::BodyPart,
                types::battle::TargetType,
                Targets,
                Damage,
                Ailments,
                SkillCost,
                RoleConstraints>;
        using OffensiveSkillMap = std::unordered_map<std::string, OffensiveSkill>;

        explicit SkillsMap(OffensiveSkillMap &&offensive_skill_map) : offensive_skills(std::move(offensive_skill_map)) {};

        SkillsMap(const SkillsMap &) = delete;
        SkillsMap(SkillsMap &&) = delete;
        SkillsMap &operator=(const SkillsMap &) = delete;
        SkillsMap &operator=(SkillsMap &&) = delete;
        ~SkillsMap() = default;

        OffensiveSkillMap offensive_skills;
    };

    SkillsMap from_json(nlohmann::json &json);
}

#endif//DUNGEON_CRAWLER_SKILLS_HPP
