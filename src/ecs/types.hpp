//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_TYPES_HPP
#define DUNGEON_CRAWLER_TYPES_HPP

#include <cstdint>
#include <unordered_map>
#include <string_view>

namespace types {
    enum class FloorType
    {
        RUINS_01, RUINS_02
    };
    enum class CeilingType {
        NORMAL
    };
    enum class WallType {
        RUINS_01
    };
    enum class DoorType {
        RUINS_01_CLOSED, RUINS_01_OPEN
    };
    enum class DoorStateType {
        CLOSED, OPEN
    };
    enum class WorldDirection {
        NORTH, EAST, WEST, SOUTH
    };
    enum class MonsterType {
        GHOUL, GOBLIN, SAND_SCORPION, SPECTRE, NOMAD_THIEF
    };

    namespace character {
        enum class Attribute {
            LEVEL, EXPERIENCE, HIT_POINTS, SKILL_POINTS, STRENGTH, INTELLIGENCE, VITALITY, WISDOM, DEXTERITY, LUCK, ATTACK, MAGIC_ATTACK, DEFENSE, MAGIC_DEFENSE
        };
        static std::unordered_map<Attribute, std::string_view> attribute_to_string = {
                {Attribute::LEVEL, "LEVEL"},
                {Attribute::EXPERIENCE, "EXPERIENCE"},
                {Attribute::HIT_POINTS, "HIT_POINTS"},
                {Attribute::SKILL_POINTS, "SKILL_POINTS"},
                {Attribute::STRENGTH, "STRENGTH"},
                {Attribute::INTELLIGENCE, "INTELLIGENCE"},
                {Attribute::VITALITY, "VITALITY"},
                {Attribute::WISDOM, "WISDOM"},
                {Attribute::DEXTERITY, "DEXTERITY"},
                {Attribute::LUCK, "LUCK"},
                {Attribute::ATTACK, "ATTACK"},
                {Attribute::MAGIC_ATTACK, "MAGIC_ATTACK"},
                {Attribute::DEFENSE, "DEFENSE"},
                {Attribute::MAGIC_DEFENSE, "MAGIC_DEFENSE"}
        };
        static std::unordered_map<std::string_view, Attribute> string_to_attribute = {
                {"LEVEL", Attribute::LEVEL},
                {"EXPERIENCE", Attribute::EXPERIENCE},
                {"HIT_POINTS", Attribute::HIT_POINTS},
                {"SKILL_POINTS", Attribute::SKILL_POINTS},
                {"STRENGTH", Attribute::STRENGTH},
                {"INTELLIGENCE", Attribute::INTELLIGENCE},
                {"VITALITY", Attribute::VITALITY},
                {"WISDOM", Attribute::WISDOM},
                {"DEXTERITY", Attribute::DEXTERITY},
                {"LUCK", Attribute::LUCK},
                {"ATTACK", Attribute::ATTACK},
                {"MAGIC_ATTACK", Attribute::MAGIC_ATTACK},
                {"DEFENSE", Attribute::DEFENSE},
                {"MAGIC_DEFENSE", Attribute::MAGIC_DEFENSE}
        };
        enum class Role {
            CULTIST, HEAVY_KNIGHT, MAGE, THIEF, WARRIOR
        };
        static std::unordered_map<Role, std::string_view> role_to_string = {
                {Role::CULTIST, "CULTIST"},
                {Role::HEAVY_KNIGHT, "HEAVY_KNIGHT"},
                {Role::MAGE, "MAGE"},
                {Role::THIEF, "THIEF"},
                {Role::WARRIOR, "WARRIOR"}
        };
        static std::unordered_map<std::string_view, Role> string_to_role = {
                {"CULTIST", Role::CULTIST},
                {"HEAVY_KNIGHT", Role::HEAVY_KNIGHT},
                {"MAGE", Role::MAGE},
                {"THIEF", Role::THIEF},
                {"WARRIOR", Role::WARRIOR}
        };
    }
namespace battle {
        enum class BattlePhase {
            INACTIVE, BATTLE_START, TURN_START, PLAYER_ACTIONS, AI_ACTIONS, TURN_END, BATTLE_END, FINISHED
        };

        enum class BattleAction {
            ATTACK, DEFEND, USE_ITEM, ESCAPE, SKILL, PARTY_SKILL, SWITCH_POSITION
        };

        enum class AttackType {
            BASH, CUT, PIERCE, FIRE, ICE, LIGHTNING, POISON, DARKNESS, HOLY
        };
        static std::unordered_map<AttackType, std::string_view> attack_type_to_string = {
                {AttackType::BASH, "BASH"},
                {AttackType::CUT, "CUT"},
                {AttackType::PIERCE, "PIERCE"},
                {AttackType::FIRE, "FIRE"},
                {AttackType::ICE, "ICE"},
                {AttackType::LIGHTNING, "LIGHTNING"},
                {AttackType::POISON, "POISON"},
                {AttackType::DARKNESS, "DARKNESS"},
                {AttackType::HOLY, "HOLY"}
        };
        static std::unordered_map<std::string_view, AttackType> string_to_attack_type = {
                {"BASH", AttackType::BASH},
                {"CUT", AttackType::CUT},
                {"PIERCE", AttackType::PIERCE},
                {"FIRE", AttackType::FIRE},
                {"ICE", AttackType::ICE},
                {"LIGHTNING", AttackType::LIGHTNING},
                {"POISON", AttackType::POISON},
                {"DARKNESS", AttackType::DARKNESS},
                {"HOLY", AttackType::HOLY}
        };
        enum class TargetType {
            ENEMY, ALLY, SELF, ALL
        };
        static std::unordered_map<TargetType, std::string_view> target_type_to_string = {
                {TargetType::ENEMY, "ENEMY"},
                {TargetType::ALLY, "ALLY"},
                {TargetType::SELF, "SELF"},
                {TargetType::ALL, "ALL"}
        };
        static std::unordered_map<std::string_view, TargetType> string_to_target_type = {
                {"ENEMY", TargetType::ENEMY},
                {"ALLY", TargetType::ALLY},
                {"SELF", TargetType::SELF},
                {"ALL", TargetType::ALL}
        };
        enum class Target {
            ALL, SELF, SINGLE, NEIGHBOURS, HORIZONTAL, VERTICAL, SINGLE_WITH_NEIGHBOURS
        };
        static std::unordered_map<Target, std::string_view> target_to_string = {
                {Target::ALL, "ALL"},
                {Target::SELF, "SELF"},
                {Target::SINGLE, "SINGLE"},
                {Target::NEIGHBOURS, "NEIGHBOURS"},
                {Target::HORIZONTAL, "HORIZONTAL"},
                {Target::VERTICAL, "VERTICAL"},
                {Target::SINGLE_WITH_NEIGHBOURS, "SINGLE_WITH_NEIGHBOURS"}
        };
        static std::unordered_map<std::string_view, Target> string_to_target = {
                {"ALL", Target::ALL},
                {"SELF", Target::SELF},
                {"SINGLE", Target::SINGLE},
                {"NEIGHBOURS", Target::NEIGHBOURS},
                {"HORIZONTAL", Target::HORIZONTAL},
                {"VERTICAL", Target::VERTICAL},
                {"SINGLE_WITH_NEIGHBOURS", Target::SINGLE_WITH_NEIGHBOURS}
        };

        enum class Ailment {
            NONE, BLEED, BLIND, BURN, CHARM, CONFUSE, CURSE, DEATH, DOOM, FEAR, FREEZE, PARALYSIS, PETRIFICATION, POISON, SILENCE, SLEEP, SLOW, STUN, WEAKNESS
        };
        static std::unordered_map<Ailment, std::string_view> ailment_to_string = {
                {Ailment::BLEED, "BLEED"},
                {Ailment::BLIND, "BLIND"},
                {Ailment::BURN, "BURN"},
                {Ailment::CHARM, "CHARM"},
                {Ailment::CONFUSE, "CONFUSE"},
                {Ailment::CURSE, "CURSE"},
                {Ailment::DEATH, "DEATH"},
                {Ailment::DOOM, "DOOM"},
                {Ailment::FEAR, "FEAR"},
                {Ailment::FREEZE, "FREEZE"},
                {Ailment::PARALYSIS, "PARALYSIS"},
                {Ailment::PETRIFICATION, "PETRIFICATION"},
                {Ailment::POISON, "POISON"},
                {Ailment::SILENCE, "SILENCE"},
                {Ailment::SLEEP, "SLEEP"},
                {Ailment::SLOW, "SLOW"},
                {Ailment::STUN, "STUN"},
                {Ailment::WEAKNESS, "WEAKNESS"}
        };
        static std::unordered_map<std::string_view, Ailment> string_to_ailment = {
                {"BLEED", Ailment::BLEED},
                {"BLIND", Ailment::BLIND},
                {"BURN", Ailment::BURN},
                {"CHARM", Ailment::CHARM},
                {"CONFUSE", Ailment::CONFUSE},
                {"CURSE", Ailment::CURSE},
                {"DEATH", Ailment::DEATH},
                {"DOOM", Ailment::DOOM},
                {"FEAR", Ailment::FEAR},
                {"FREEZE", Ailment::FREEZE},
                {"PARALYSIS", Ailment::PARALYSIS},
                {"PETRIFICATION", Ailment::PETRIFICATION},
                {"POISON", Ailment::POISON},
                {"SILENCE", Ailment::SILENCE},
                {"SLEEP", Ailment::SLEEP},
                {"SLOW", Ailment::SLOW},
                {"STUN", Ailment::STUN},
                {"WEAKNESS", Ailment::WEAKNESS}
        };

        enum class BodyPart {
            ARMS, LEGS, HEAD
        };
        static std::unordered_map<BodyPart, std::string_view> body_part_to_string = {
                {BodyPart::ARMS, "ARMS"},
                {BodyPart::LEGS, "LEGS"},
                {BodyPart::HEAD, "HEAD"}
        };
        static std::unordered_map<std::string_view, BodyPart> string_to_body_part = {
                {"ARMS", BodyPart::ARMS},
                {"LEGS", BodyPart::LEGS},
                {"HEAD", BodyPart::HEAD}
        };
    }
}

#endif//DUNGEON_CRAWLER_TYPES_HPP
