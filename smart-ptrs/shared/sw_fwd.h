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
        return count_;
    }
    void Inc() {
        ++count_;
    }
    void Dec() {
        --count_;
        if (count_ == 0) {
            Delete();
        }
    }
    virtual ~ControlBlock() = default;

protected:
    size_t count_ = 0;

private:
    virtual void Delete() = 0;
};

template <typename T>
class CBNoPtr : public ControlBlock {
public:
    template <class... Args>
    CBNoPtr(Args&&... args) {
        ::new (&object_) T(std::forward<Args>(args)...);
        count_ = 1;
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
        count_ = 1;
    }

    CBWithPtr(const CBWithPtr& obj) {
        object_ptr_ = obj.object_ptr_;
        count_ = obj.count_;
    }

    template <class S>
    CBWithPtr(const CBWithPtr<S>& obj) {
        object_ptr_ = obj.object_ptr_;
        count_ = obj.count_;
    }

    CBWithPtr& operator=(CBWithPtr& obj) {
        if (&obj == this) {
            return *this;
        }
        object_ptr_ = obj.object_ptr_;
        count_ = obj.count_;
        return *this;
    }

    template <class S>
    CBWithPtr& operator=(CBWithPtr<S>& obj) {
        if (&obj == this) {
            return *this;
        }
        object_ptr_ = obj.object_ptr_;
        count_ = obj.count_;
        return *this;
    }

private:
    T* object_ptr_;

    void Delete() override {
        delete object_ptr_;
    }
};