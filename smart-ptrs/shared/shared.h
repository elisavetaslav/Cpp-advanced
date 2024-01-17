#pragma once

#include <memory>

#include "sw_fwd.h"  // Forward declaration

#include <cstddef>  // std::nullptr_t

template <typename T>
class SharedPtr {
public:
    // Constructors

    SharedPtr() {
        block_ = nullptr;
        ptr_ = nullptr;
    }
    SharedPtr(std::nullptr_t) {
        block_ = std::nullptr_t();
        ptr_ = std::nullptr_t();
    }
    explicit SharedPtr(T* ptr) {
        ptr_ = ptr;
        block_ = new CBWithPtr(ptr);
    }
    /// template
    template <class S>
    explicit SharedPtr(S* ptr) {
        ptr_ = ptr;
        block_ = new CBWithPtr(ptr);
    }
    SharedPtr(const SharedPtr& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        SafeInc();
    }

    template <class S>
    SharedPtr(const SharedPtr<S>& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        SafeInc();
    }

    SharedPtr(SharedPtr&& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    template <class S>
    SharedPtr(SharedPtr<S>&& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }

    explicit SharedPtr(CBNoPtr<T>* cb) {
        ptr_ = cb->GetPtr();
        block_ = cb;
    }
    // Aliasing constructor
    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other, T* ptr) {
        block_ = other.block_;
        ptr_ = ptr;
        SafeInc();
    }

    // `operator=`-s

    SharedPtr& operator=(const SharedPtr& other) {
        if (&other == this) {
            return *this;
        }
        if (block_) {
            SafeDec();
            block_ = nullptr;
            ptr_ = nullptr;
        }
        block_ = other.block_;
        SafeInc();
        ptr_ = other.ptr_;
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (&other == this) {
            return *this;
        }
        if (block_) {
            SafeDec();
            block_ = nullptr;
            ptr_ = nullptr;
        }
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
        return *this;
    }

    // Destructor

    ~SharedPtr() {
        if (block_) {
            if (block_->Get() > 0) {
                block_->Dec();
            }
            if (block_->Get() == 0) {
                delete block_;
                ptr_ = nullptr;
                block_ = nullptr;
            }
        }
    }

    // Modifiers

    void Reset() {
        SafeDec();
        block_ = nullptr;
        ptr_ = nullptr;
    };
    void Reset(T* ptr) {
        SafeDec();
        ptr_ = ptr;
        block_ = new CBWithPtr(ptr);
    }

    template <class S>
    void Reset(S* ptr) {
        SafeDec();
        ptr_ = ptr;
        block_ = new CBWithPtr<S>(ptr);
    }

    void Swap(SharedPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
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
        if (block_) {
            return block_->Get();
        }
        return 0;
    }
    explicit operator bool() const {
        return ptr_ != nullptr;
    }

private:
    template <typename Y>
    friend class SharedPtr;

    void SafeDec() {
        if (block_) {
            block_->Dec();
            if (block_->Get() == 0) {
                delete block_;
            }
        }
    }

    void SafeInc() {
        if (block_) {
            block_->Inc();
        }
    }

    ControlBlock* block_;
    T* ptr_;
};

template <typename T, typename U>
inline bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right) {
    return left.Get() == right.Get();
};

// Allocate memory only once
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new CBNoPtr<T>(std::forward<Args>(args)...));
};

// Look for usage examples in tests
template <typename T>
class EnableSharedFromThis {
public:
    SharedPtr<T> SharedFromThis();
    SharedPtr<const T> SharedFromThis() const;

    WeakPtr<T> WeakFromThis() noexcept;
    WeakPtr<const T> WeakFromThis() const noexcept;
};