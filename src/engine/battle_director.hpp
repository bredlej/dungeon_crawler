//
// Created by Patryk Szczypień on 15/08/2023.
//

#ifndef DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
#define DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
#include <engine/core.hpp>
#include <memory>

enum class BattlePhase {
    INACTIVE,
    BATTLE_START,
    TURN_START,
    PLAYER_ACTIONS,
    AI_ACTIONS,
    TURN_END,
    BATTLE_END,
    FINISHED
};

struct NextStateEvent {
    BattlePhase from_phase;
};

template<typename T>
class BattleDirector {
public:
    explicit BattleDirector(std::shared_ptr<Core> &core) : _core{core} {};
    virtual ~BattleDirector() = default;

    [[nodiscard]] BattlePhase get_battle_phase() const noexcept {
        return _battle_phase;
    }

    virtual constexpr void update() noexcept {}
    virtual constexpr void next_state(const NextStateEvent &next_state_event) noexcept {}
protected:
    std::shared_ptr<Core> _core;

    virtual void initialize() noexcept {
        static_cast<T *>(this)->initialize();
    };

    virtual void pre_battle_start() noexcept {
        static_cast<T *>(this)->pre_battle_start();
    };
    virtual void battle_start() noexcept {
        static_cast<T *>(this)->battle_start();
    }
    virtual void post_battle_start() noexcept {
        static_cast<T *>(this)->post_battle_start();
    }

    virtual bool battle_start_guard() noexcept {
        return static_cast<T *>(this)->battle_start_guard();
    }

    virtual void pre_turn_start() noexcept {
        static_cast<T *>(this)->pre_turn_start();
    };
    virtual void turn_start() noexcept {
        static_cast<T *>(this)->turn_start();
    }
    virtual void post_turn_start() noexcept {
        static_cast<T *>(this)->post_turn_start();
    }

    virtual bool turn_start_guard() noexcept {
        return static_cast<T *>(this)->turn_start_guard();
    }

    virtual void pre_player_actions() noexcept {
        static_cast<T *>(this)->pre_player_actions();
    };
    virtual void player_actions() noexcept {
        static_cast<T *>(this)->player_actions();
    }
    virtual void post_player_actions() noexcept {
        static_cast<T *>(this)->post_player_actions();
    }

    virtual bool player_actions_guard() noexcept {
        return static_cast<T *>(this)->player_actions_guard();
    }

    virtual void pre_ai_actions() noexcept {
        static_cast<T *>(this)->pre_ai_actions();
    };
    virtual void ai_actions() noexcept {
        static_cast<T *>(this)->ai_actions();
    }
    virtual void post_ai_actions() noexcept {
        static_cast<T *>(this)->post_ai_actions();
    }

    virtual bool ai_actions_guard() noexcept {
        return static_cast<T *>(this)->ai_actions_guard();
    }

    virtual void pre_turn_end() noexcept {
        static_cast<T *>(this)->pre_turn_end();
    };
    virtual void turn_end() noexcept {
        static_cast<T *>(this)->turn_end();
    }
    virtual void post_turn_end() noexcept {
        static_cast<T *>(this)->post_turn_end();
    }

    virtual bool turn_end_guard() noexcept {
        return static_cast<T *>(this)->turn_end_guard();
    }

    virtual bool end_condition() noexcept {
        return static_cast<T *>(this)->end_condition();
    }

    virtual void pre_battle_end() noexcept {
        static_cast<T *>(this)->pre_battle_end();
    };
    virtual void battle_end() noexcept {
        static_cast<T *>(this)->battle_end();
    }

    virtual void post_battle_end() noexcept {
        static_cast<T *>(this)->post_battle_end();
    }

    virtual bool battle_end_guard() noexcept {
        return static_cast<T *>(this)->battle_end_guard();
    }

    virtual void finish() noexcept {
        static_cast<T *>(this)->finish();
    };

    BattlePhase _battle_phase{BattlePhase::INACTIVE};
};

#endif//DUNGEON_CRAWLER_BATTLE_DIRECTOR_HPP
