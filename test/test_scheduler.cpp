//
// Created by Patryk Szczypień on 22/08/2024.
//
#include "scheduler.hpp"
#include <gtest/gtest.h>

class SchedulerTest : public testing::Test {
protected:
    SchedulerTest() = default;
};

TEST_F(SchedulerTest, Test_Scheduler_Runs_Once) {
    std::shared_ptr<Core> core = std::make_shared<Core>();

    struct Timer {
        int32_t seconds = 0;
    };
    core->registry.ctx().emplace<Timer>();

    Scheduler<std::chrono::milliseconds> scheduler(core);
    scheduler.builder()
            .after(1, [&core](const std::shared_ptr<Core> &) {
                auto *timer = core->registry.ctx().find<Timer>();
                timer->seconds += 1;
            })
            .after(2, [&core](const std::shared_ptr<Core> &) {
                auto *timer = core->registry.ctx().find<Timer>();
                timer->seconds += 2;
            })
            .once()
            .run();

    auto *timer = core->registry.ctx().find<Timer>();

    while (true) {
        if (timer->seconds == 3) {
            break;
        }
    }
    ASSERT_EQ(3, timer->seconds);
}

TEST_F(SchedulerTest, Test_Scheduler_Runs_Repeating) {
    std::shared_ptr<Core> core = std::make_shared<Core>();

    struct Timer {
        int32_t seconds = 0;
    };
    core->registry.ctx().emplace<Timer>();

    Scheduler<std::chrono::milliseconds> scheduler(core);
    scheduler.builder()
            .after(1, [&core](const std::shared_ptr<Core> &) {
                auto *timer = core->registry.ctx().find<Timer>();
                timer->seconds += 1;
            })
            .after(2, [&core](const std::shared_ptr<Core> &) {
                auto *timer = core->registry.ctx().find<Timer>();
                timer->seconds += 2;
            })
            .repeating()
            .run();

    auto *timer = core->registry.ctx().find<Timer>();

    while (true) {
        if (timer->seconds == 9) {
            break;
        }
    }

    ASSERT_EQ(9, timer->seconds);
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}