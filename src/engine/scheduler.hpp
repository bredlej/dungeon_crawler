//
// Created by Patryk Szczypień on 22/08/2024.
//

#ifndef DUNGEON_CRAWLER_SCHEDULER_HPP
#define DUNGEON_CRAWLER_SCHEDULER_HPP

#include <chrono>
#include <condition_variable>
#include <engine/core.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class Core;

template<typename TIME_UNIT>
class Scheduler {
public:
    class Builder {
    public:
        explicit Builder(Scheduler *scheduler) : _scheduler(scheduler) {}

        Builder &after(int delay, const std::function<void(const std::shared_ptr<Core> &)> &task) {
            _scheduler->_tasks.emplace_back(to_duration(delay), task);
            return *this;
        }

        Builder &repeating() {
            _scheduler->_repeating = true;
            return *this;
        }

        Builder &once() {
            _scheduler->_repeating = false;
            return *this;
        }

        void run() {
            _scheduler->run();
        }

    private:
        Scheduler<TIME_UNIT> *_scheduler;

        TIME_UNIT to_duration(int delay) {
            return TIME_UNIT(delay);
        }
    };

public:
    explicit Scheduler(const std::shared_ptr<Core> &core) : _core(core), _repeating(false), _stop(false) {}

    ~Scheduler() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _stop = true;
        }
        _cv.notify_all();
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    Builder builder() {
        return Builder(this);
    }

    void run() {
        _thread = std::thread([this]() {
            std::unique_lock<std::mutex> lock(_mutex);
            while (!_stop) {
                for (const auto &[delay, task]: _tasks) {
                    _cv.wait_for(lock, delay, [this]() {
                        return _stop;
                    });
                    if (_stop) break;
                    task(_core);
                }
                if (!_repeating) {
                    break;
                }
            }
        });
    }

private:
    std::vector<std::pair<TIME_UNIT, std::function<void(const std::shared_ptr<Core> &)>>> _tasks;
    bool _repeating;
    std::shared_ptr<Core> _core;
    bool _stop {};
    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _cv;
};
#endif//DUNGEON_CRAWLER_SCHEDULER_HPP
