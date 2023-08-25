//
// Created by geoco on 24.11.2022.
//

#ifndef DUNGEON_CRAWLER_COMPONENTS_HPP
#define DUNGEON_CRAWLER_COMPONENTS_HPP
#include <cstdint>
#include <ecs/types.hpp>
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>

namespace components {
    namespace general {
        struct Direction {
            types::WorldDirection direction;
        };
        struct Player {
            bool is_active;
        };
        struct Name {
            std::string name;
        };
        struct Skill {};
        struct SkillCost {
            int sp;
            int hp;
        };
    }
    namespace values {
        struct EncounterChance {
            float chance;
        };
        struct Encounter {};
        struct Tint {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
    }
    namespace tiles {
        struct MapPosition {
            int32_t x;
            int32_t y;
        };
        struct TileId {
            int id;
        };
        struct Floor {
            types::FloorType type;
        };
        struct Ceiling {
            types::CeilingType type;
        };
        struct Wall {
            types::WallType type;
            MapPosition field1;
            MapPosition field2;
        };
        struct Door {
            types::DoorType type_closed;
            types::DoorType type_opened;
            types::DoorStateType state;
        };
        struct Visibility {
            bool is_visible;
        };
        struct Walkability {
            bool walkable;
        };
        struct InFovOfEntity {
            entt::entity entity;
        };
    }
    namespace battle {
        namespace targets {
            struct TargetSingle {
                entt::entity target;
                float percentage;
            };
            struct TargetMultiple {
                explicit TargetMultiple(const std::vector<entt::entity> &targets, float percentage) : targets{targets}, percentage{percentage} {}
                std::vector<entt::entity> targets;
                float percentage;
            };
        }

        namespace damage_types {
            struct FireDmg {
                float damage;
                static constexpr std::string_view description = "fire damage";
            };
        }

        namespace mods {
            template <typename DAMAGE_TYPE>
            struct Weakness {
                float damage_multiplier;
            };

            template <typename DAMAGE_TYPE>
            struct Resistance {
                float damage_multiplier;
            };

            template <typename DAMAGE_TYPE>
            struct Immunity {
                float damage_multiplier;
            };
        }

        namespace ailments {
            struct Death {
                bool is_undead{false};
                static constexpr std::string_view description = "is dead!";
            };
            struct Burn {
                float chance;
                float damage;
                uint32_t duration;
                static constexpr std::string_view description = "is burning!";
            };
        }

        struct Attributes {
            std::unordered_map<types::character::Attribute, float> attributes;
        };

        struct Neighbours {
            std::vector<entt::entity> neighbours;
        };

        struct RoleConstraints {
            std::vector<types::character::Role> roles;
        };

        struct BodyConstraints {
            std::vector<types::battle::BodyPart> body_parts;
        };
    }
}
#endif//DUNGEON_CRAWLER_COMPONENTS_HPP
