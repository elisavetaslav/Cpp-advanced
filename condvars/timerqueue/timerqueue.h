#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <set>
#include <tuple>

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

public:
    void Add(const T& item, TimePoint at) {
        std::lock_guard<std::mutex> lock(m_);
        queue_.insert({at, item});
        t_min_ = queue_.begin()->first;
        cv_.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lk(m_);
        if (queue_.size() > 0) {
            cv_.wait_until(lk, t_min_);
            T ans = queue_.begin()->second;
            queue_.erase(queue_.begin());
            return ans;
        } else {
            cv_.wait(lk);
            T ans = queue_.begin()->second;
            return ans;
        }
    }

private:
    std::set<std::pair<TimePoint, const T>> queue_;
    TimePoint t_min_ = TimePoint::max();
    std::mutex m_;
    std::condition_variable cv_;
};