#pragma once

#include "sw_fwd.h"  // Forward declaration

template <typename T>
class WeakPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    WeakPtr() {
        block_ = nullptr;
        ptr_ = nullptr;
    }

    WeakPtr(const WeakPtr& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        SafeWeakInc();
    }

    template <class S>
    WeakPtr(const WeakPtr<S>& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        SafeWeakInc();
    }

    WeakPtr(WeakPtr&& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        other.block_ = nullptr;
        other.ptr_ = nullptr;
    }

    template <class S>
    WeakPtr(WeakPtr<S>&& other) {
        block_ = other.block_;
        ptr_ = other.ptr_;
        block_ = nullptr;
        ptr_ = nullptr;
    }

    // Demote `SharedPtr`
    WeakPtr(const SharedPtr<T>& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        SafeWeakInc();
    }

    template <class S>
    WeakPtr(const SharedPtr<S>& other) {
        ptr_ = other.ptr_;
        block_ = other.block_;
        SafeWeakInc();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    WeakPtr& operator=(const WeakPtr& other) {
        if (&other == this) {
            return *this;
        }
        SafeWeakDec();
        block_ = other.block_;
        SafeWeakInc();
        ptr_ = other.ptr_;
        return *this;
    }
    WeakPtr& operator=(WeakPtr&& other) {
        if (&other == this) {
            return *this;
        }
        SafeWeakDec();
        block_ = other.block_;
        SafeWeakInc();
        ptr_ = other.ptr_;
        if (other.block_) {
            if (other.block_->WeakGet() > 0) {
                other.block_->WeakDec();
            }
            if (other.block_->WeakGet() + other.block_->Get() == 0) {
                delete other.block_;
                other.ptr_ = nullptr;
                other.block_ = nullptr;
            }
        }
        other.block_ = nullptr;
        other.ptr_ = nullptr;
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~WeakPtr() {
        SafeWeakDec();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        SafeWeakDec();
        block_ = nullptr;
        ptr_ = nullptr;
    }
    void Swap(WeakPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    size_t UseCount() const {
        if (block_) {
            return block_->Get();  // WeakGet()
        }
        return 0;
    }
    bool Expired() const {
        if (UseCount() == 0) {
            return true;
        }
        return false;
    }
    SharedPtr<T> Lock() const {
        if (Expired()) {
            return SharedPtr<T>();
        }
        return SharedPtr<T>(*this);
    }

protected:
    ControlBlock* block_;
    T* ptr_;

private:
    template <typename Y>
    friend class SharedPtr;

    template <typename Y>
    friend class WeakPtr;

    void SafeWeakInc() {
        if (block_) {
            block_->WeakInc();
        }
    }

    void SafeWeakDec() {
        if (block_) {
            if (block_->WeakGet() > 0) {
                block_->WeakDec();
            }
            if (block_->WeakGet() + block_->Get() == 0) {
                delete block_;
                ptr_ = nullptr;
                block_ = nullptr;
            }
        }
    }
};