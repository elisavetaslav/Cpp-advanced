#pragma once

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#include <atomic>

// Atomically do the following:
//    if (*value == expected_value) {
//        sleep_on_address(value)
//    }
void FutexWait(int *value, int expected_value) {
    syscall(SYS_futex, value, FUTEX_WAIT_PRIVATE, expected_value, nullptr, nullptr, 0);
}

// Wakeup 'count' threads sleeping on address of value(-1 wakes all)
void FutexWake(int *value, int count) {
    syscall(SYS_futex, value, FUTEX_WAKE_PRIVATE, count, nullptr, nullptr, 0);
}

int Cmp(std::atomic<int> *val, int c, int a = 1) {
    int *cp = &c;
    std::atomic_compare_exchange_strong(val, cp, a);
    return *cp;
}

class Mutex {
public:
    void Lock() {
        int c = 0, c1 = 1, c2 = 2;
        c = Cmp(&val_, c, c1);
        if (c != 2 && c != 0) {
            c = val_.exchange(c2);
        }
        while (c != 0) {
            FutexWait(reinterpret_cast<int *>(&val_), c2);
            c = val_.exchange(c2);
        }
    }
    void Unlock() {
        int c1 = 1;
        int c = val_.fetch_sub(c1);
        if (c != 1) {
            val_ = 0;
            FutexWake(reinterpret_cast<int *>(&val_), c1);
        }
    }

private:
    std::atomic<int> val_{0};
};