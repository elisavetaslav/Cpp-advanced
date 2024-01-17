#pragma once

#include <cstddef>  // for std::nullptr_t
#include <utility>  // for std::exchange / std::swap

class SimpleCounter {
public:
    size_t IncRef() {
        return ++count_;
    }
    size_t DecRef() {
        return --count_;
    }
    size_t RefCount() const {
        return count_;
    }

private:
    size_t count_ = 0;
};

struct DefaultDelete {
    template <typename T>
    static void Destroy(T* object) {
        delete object;
    }
};

template <typename Derived, typename Counter, typename Deleter>
class RefCounted {
public:
    // Increase reference counter.
    void IncRef() {
        counter_.IncRef();
    }

    // Decrease reference counter.
    // Destroy object using Deleter when the last instance dies.
    void DecRef() {
        if (counter_.RefCount() == 0 || counter_.RefCount() == 1) {
            Deleter::Destroy(static_cast<Derived*>(this));
        } else {
            counter_.DecRef();
        }
    }

    // Get current counter value (the number of strong references).
    size_t RefCount() const {
        return counter_.RefCount();
    }

private:
    Counter counter_;
};

template <typename Derived, typename D = DefaultDelete>
using SimpleRefCounted = RefCounted<Derived, SimpleCounter, D>;

template <typename T>
class IntrusivePtr {
    template <typename Y>
    friend class IntrusivePtr;

public:
    // Constructors
    IntrusivePtr() {
        ptr_ = nullptr;
    }
    IntrusivePtr(std::nullptr_t) {
        ptr_ = std::nullptr_t();
    }
    IntrusivePtr(T* ptr) {
        ptr_ = ptr;
        SafeInc();
    }

    template <class S>
    IntrusivePtr(S* ptr) {
        ptr_ = ptr;
        SafeInc();
    }

    template <typename Y>
    IntrusivePtr(const IntrusivePtr<Y>& other) {
        ptr_ = other.ptr_;
        SafeInc();
    }

    template <typename Y>
    IntrusivePtr(IntrusivePtr<Y>&& other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    IntrusivePtr(const IntrusivePtr& other) {
        ptr_ = other.ptr_;
        SafeInc();
    }
    IntrusivePtr(IntrusivePtr&& other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    // `operator=`-s
    IntrusivePtr& operator=(const IntrusivePtr& other) {
        if (&other == this) {
            return *this;
        }
        SafeDec();
        ptr_ = other.ptr_;
        SafeInc();
        return *this;
    }

    IntrusivePtr& operator=(IntrusivePtr&& other) {
        if (&other == this) {
            return *this;
        }
        SafeDec();
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    // Destructor
    ~IntrusivePtr() {
        SafeDec();
    }

    // Modifiers
    void Reset() {
        SafeDec();
        ptr_ = nullptr;
    }
    void Reset(T* ptr) {
        SafeDec();
        ptr_ = ptr;
    }
    void Swap(IntrusivePtr& other) {
        std::swap(ptr_, other.ptr_);
    }

    // Observers
    T* Get() const {
        return ptr_;
    }
    T& operator*() const {
        return *ptr_;
    }
    T* operator->() const {
        return ptr_;
    }
    size_t UseCount() const {
        if (!ptr_) {
            return 0;
        }
        return ptr_->RefCount();
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    }

private:
    T* ptr_;

    void SafeInc() {
        if (ptr) {
            ptr_->IncRef();
        }
    }

    void SafeDec() {
        if (ptr_) {
            ptr_->DecRef();
        }
    }
};

template <typename T, typename... Args>
IntrusivePtr<T> MakeIntrusive(Args&&... args) {
    return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
}