#pragma once

#include <algorithm>

class ListHook {
public:
    ListHook() {
        al_after_ = this;
        al_before_ = this;
    }

    bool IsLinked() const {
        return (al_after_ != this && al_before_ != this);
    }

    void Unlink() {
        al_before_->al_after_ = al_after_;
        al_after_->al_before_ = al_before_;
        al_after_ = this;
        al_before_ = this;
    }

    ~ListHook() {
        Unlink();
    }

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;

    ListHook* al_after_ = this;
    ListHook* al_before_ = this;

    void LinkBefore(ListHook* other) {
        al_before_ = other->al_before_;
        other->al_before_->al_after_ = this;
        other->al_before_ = this;
        al_after_ = other;
    }
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(T* ptr) {
            pole_ = ptr;
        }

        Iterator& operator++() {
            pole_ = static_cast<T*>(pole_->al_after_);
            return *this;
        }

        Iterator operator++(int) {
            Iterator pred = *this;
            pole_ = static_cast<T*>(pole_->al_after_);
            return pred;
        }

        T& operator*() const {
            return *pole_;
        }

        T* operator->() const {
            return pole_;
        }

        bool operator==(const Iterator& rhs) const {
            return pole_ == rhs.pole_;
        }

        bool operator!=(const Iterator& rhs) const {
            return pole_ != rhs.pole_;
        }

    private:
        T* pole_;
    };

    List() {
        dummy_.al_after_ = &dummy_;
        dummy_.al_before_ = &dummy_;
    }

    List(const List&) = delete;

    List(List&& other) {
        dummy_ = std::move(other.dummy_);
        other.dummy_.al_after_ = &other.dummy_;
        other.dummy_.al_before_ = &other.dummy_;
    }

    ~List() {
        ListHook* cur = dummy_.al_after_;
        while (dummy_.al_after_ != &dummy_) {
            ListHook* next = cur->al_after_;
            cur->Unlink();
            cur = next;
        }
    };

    List& operator=(const List&) = delete;

    List& operator=(List&& other) {
        if (this != &other) {
            dummy_ = std::move(other.dummy_);
            other.dummy_.al_after_ = &other.dummy_;
            other.dummy_.al_before_ = &other.dummy_;
        }
        return *this;
    }

    bool IsEmpty() const {
        return (!dummy_.IsLinked());
    }

    size_t Size() const {
        size_t count = 0;
        for (ListHook* cur = dummy_.al_after_; cur != &dummy_; cur = cur->al_after_) {
            ++count;
        }
        return count;
    }

    void PushBack(T* elem) {
        elem->LinkBefore(&dummy_);
    }

    void PushFront(T* elem) {
        elem->LinkBefore(dummy_.al_after_);
    }

    T& Front() {
        return *static_cast<T*>(dummy_.al_after_);
    }

    const T& Front() const {
        return *static_cast<const T*>(dummy_.al_after_);
    }

    T& Back() {
        return *static_cast<T*>(dummy_.al_before_);
    }

    const T& Back() const {
        return *static_cast<const T*>(dummy_.al_before_);
    }

    void PopBack() {
        dummy_.al_before_->Unlink();
    }

    void PopFront() {
        dummy_.al_after_->Unlink();
    }

    Iterator Begin() {
        return static_cast<T*>(dummy_.al_after_);
    }

    Iterator End() {
        return static_cast<T*>(&dummy_);
    }

    Iterator IteratorTo(T* element) {
        return Iterator(element);
    }

private:
    ListHook dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}