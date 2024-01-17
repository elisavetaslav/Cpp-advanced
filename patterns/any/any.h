#pragma once
#include <memory>

class Any {
public:
    Any() {
    }

    template <class T>
    Any(const T& value) {
        base_ = new Inner<T>(value);
    }

    Any(const Any& rhs) {
        Clear();
        base_ = rhs.base_->Copy();
    }

    template <class T>
    Any& operator=(const T& value) {
        Clear();
        base_ = new Inner<T>(value);
        return *this;
    }

    Any& operator=(Any& rhs) {
        if (this != &rhs) {
            Any c(rhs);
            Swap(c);
        }
        return *this;
    }

    ~Any() {
        delete base_;
        base_ = nullptr;
    }

    bool Empty() const {
        return !base_;
    }

    void Clear() {
        delete base_;
        base_ = nullptr;
    }
    void Swap(Any& rhs) {
        std::swap(base_, rhs.base_);
    }
    template <class T>
    const T& GetValue() const {
        if (base_->Type() != typeid(T)) {
            throw std::bad_cast();
        }
        return static_cast<Inner<T>*>(base_)->Get();
    }

private:
    struct InnerBase {
        InnerBase() {
        }
        virtual const std::type_info& Type() const = 0;
        virtual InnerBase* Copy() const = 0;
        virtual ~InnerBase() {
        }
    };

    template <typename Ty>
    struct Inner : InnerBase {
        Inner(Ty newval) : value_(newval) {
        }
        InnerBase* Copy() const {
            return new Inner(value_);
        }
        const std::type_info& Type() const {
            return typeid(value_);
        }
        Ty& Get() {
            return value_;
        }
        const Ty& Get() const {
            return value_;
        }

    private:
        Ty value_;
    };

    InnerBase* base_ = nullptr;
};