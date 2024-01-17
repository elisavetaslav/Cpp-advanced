#pragma once

#include <exception>
#include <memory>

class BadWeakPtr : public std::exception {};

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

class ControlBlock {
public:
    size_t Get() const {
        return count_strong_;
    }
    void Inc() {
        ++count_strong_;
    }
    void Dec() {
        --count_strong_;
        if (count_strong_ == 0) {
            Delete();
        }
    }
    size_t WeakGet() const {
        return count_weak_;
    }
    void WeakInc() {
        ++count_weak_;
    }
    void WeakDec() {
        --count_weak_;
    }

    virtual ~ControlBlock() = default;

protected:
    size_t count_strong_ = 0;
    size_t count_weak_ = 0;

private:
    virtual void Delete() = 0;
};

template <typename T>
class CBNoPtr : public ControlBlock {
public:
    template <class... Args>
    CBNoPtr(Args&&... args) {
        ::new (&object_) T(std::forward<Args>(args)...);
        count_strong_ = 1;
    }

    T* GetPtr() {
        return reinterpret_cast<T*>(&object_);
    }

private:
    void Delete() override {
        std::destroy_at(std::launder(reinterpret_cast<T*>(&object_)));  //область не пуста
    }
    std::aligned_storage_t<sizeof(T), alignof(T)> object_;
};


template <typename T>
class CBWithPtr : public ControlBlock {
public:
    CBWithPtr(T* ptr) {
        object_ptr_ = ptr;
        count_strong_ = 1;
    }

    CBWithPtr(const CBWithPtr& obj) {
        object_ptr_ = obj.object_ptr_;
        count_strong_ = obj.count_strong_;
    }

    template <class S>
    CBWithPtr(const CBWithPtr<S>& obj) {
        object_ptr_ = obj.object_ptr_;
        count_strong_ = obj.count_strong_;
    }

    CBWithPtr& operator=(CBWithPtr& obj) {
        if (&obj == this) {
            return *this;
        }
        object_ptr_ = obj.object_ptr_;
        count_strong_ = obj.count_strong_;
        return *this;
    }

    template <class S>
    CBWithPtr& operator=(CBWithPtr<S>& obj) {
        if (&obj == this) {
            return *this;
        }
        object_ptr_ = obj.object_ptr_;
        count_strong_ = obj.count_strong_;
        return *this;
    }

private:
    T* object_ptr_;

    void Delete() override {
        delete object_ptr_;
    }
};