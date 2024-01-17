#pragma once

#include <cstring>
#include <string>

class StringView {
public:
    StringView(const std::string &s, size_t pos_start = 0, size_t len_s = std::string::npos) {
        s_begin_ = &s[pos_start];
        if (!(len_s + 1)) {
            len_s = s.length() - pos_start;
        }
        len_ = len_s;
    }

    StringView(const char *c) {
        s_begin_ = c;
        len_ = std::strlen(c);
    }

    StringView(const char *c, size_t len_s) {
        s_begin_ = c;
        len_ = len_s;
    }

    char operator[](size_t i) const {
        return s_begin_[i];
    };

    size_t Size() const {
        return len_;
    }

private:
    const char *s_begin_ = nullptr;
    size_t len_ = 0;
};