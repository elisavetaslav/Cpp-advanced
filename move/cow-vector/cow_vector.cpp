#include "cow_vector.h"

COWVector::COWVector() {
    state_ = new State;
    ++state_->ref_count;
}
COWVector::~COWVector() {
    if (state_) {
        --state_->ref_count;
        if (state_->ref_count == 0) {
            delete state_;
        }
    }
}

COWVector::COWVector(const COWVector& other) {
    state_ = other.state_;
    if (state_) {
        ++state_->ref_count;
    }
}
COWVector::COWVector(COWVector&& other) {
    if (state_) {
        --state_->ref_count;
        if (state_->ref_count <= 0) {
            delete state_;
        }
    }
    std::swap(state_, other.state_);
}
COWVector& COWVector::operator=(const COWVector& other) {
    if (this != &other) {
        if (state_) {
            --state_->ref_count;
            if (state_->ref_count <= 0) {
                delete state_;
            }
        }
        state_ = other.state_;
        if (state_) {
            ++state_->ref_count;
        }
    }
    return *this;
}

COWVector& COWVector::operator=(COWVector&& other) {
    if (this != &other) {
        if (state_) {
            --state_->ref_count;
        }
        if (state_ && state_->ref_count <= 0) {
            delete state_;
        }
        std::swap(other.state_, state_);
    }
    return *this;
}

size_t COWVector::Size() const {
    if (state_) {
        return state_->v.size();
    } else {
        return 0;
    }
}

void COWVector::Resize(size_t size) {
    if (state_ && state_->ref_count > 1) {
        DeepCopy();
    }
    if (state_) {
        state_->v.resize(size);
    }
}

const std::string& COWVector::Get(size_t at) {
    return state_->v[at];
}
const std::string& COWVector::Back() {
    return state_->v[state_->v.size() - 1];
}

void COWVector::PushBack(const std::string& value) {
    if (state_->ref_count > 1) {
        DeepCopy();
    }
    state_->v.push_back(value);
}

void COWVector::Set(size_t at, const std::string& value) {
    if (state_->ref_count > 1) {
        DeepCopy();
    }
    state_->v[at] = value;
}

void COWVector::DeepCopy() {
    State* st = new State;
    ++st->ref_count;
    if (state_) {
        --state_->ref_count;
        if (state_->ref_count <= 0) {
            delete state_;
        }
    }
    st->v = state_->v;
    state_ = st;
}