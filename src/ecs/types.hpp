//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_TYPES_HPP
#define DUNGEON_CRAWLER_TYPES_HPP

#include <cstdint>

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
        enum class Role {
            CULTIST, HEAVY_KNIGHT, MAGE, THIEF, WARRIOR
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

        enum class Target {
            ALL, SELF, SINGLE, HORIZONTAL, VERTICAL, SINGLE_WITH_NEIGHBOURS
        };

        enum class Ailment {
            NONE, BLEED, BLIND, BURN, CHARM, CONFUSE, CURSE, DEATH, DOOM, FEAR, FREEZE, PARALYSIS, PETRIFICATION, POISON, SILENCE, SLEEP, SLOW, STUN, WEAKNESS
        };

        enum class BodyPart {
            ARMS, LEGS, HEAD
        };
    }
}

#endif//DUNGEON_CRAWLER_TYPES_HPP
