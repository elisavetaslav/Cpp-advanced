#pragma once

#include <memory>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    void Set(int a) {
        number_ = a;
    }
    int GetValue() const {
        return number_;
    }

private:
    int number_;
};

class Symbol : public Object {
public:
    void Set(std::string& s) {
        string_ = s;
    }
    const std::string& GetName() const {
        return string_;
    }

private:
    std::string string_;
};

class Cell : public Object {
public:
    Cell() {
        first_ = nullptr;
        second_ = nullptr;
    }
    void Set(std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
        first_ = first;
        second_ = second;
    }
    void SetFirst(std::shared_ptr<Object> first) {
        first_ = first;
    }
    void SetSecond(std::shared_ptr<Object> second) {
        second_ = second;
    }
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }
    Cell* operator=(const Cell& other) {
        first_ = other.first_;
        second_ = other.second_;
        return this;
    }

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and convertion.

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return dynamic_cast<T*>(obj.get()) != nullptr;
}