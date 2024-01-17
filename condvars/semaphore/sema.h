#pragma once

#include <mutex>
#include <condition_variable>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    Semaphore(int count) : count_(count) {
    }

    void Leave() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        ++incoming_;
        cv_.notify_one();
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        int current_incoming = incoming_;
        ++incoming_;
        while (current_incoming != outgoing_ && !incoming_) {
            cv_.wait(lock);
        }
        outgoing_--;
        callback(count_);
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int outgoing_ = 0;
    int count_ = 0;
    int incoming_ = 0;
};