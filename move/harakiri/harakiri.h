#pragma once

#include <string>

class OneTimeCallback {
public:
    virtual ~OneTimeCallback() = default;
    virtual std::string operator()() const&& = 0;
};

class AwesomeCallback : public OneTimeCallback {
public:
    AwesomeCallback(std::string&& s);

    std::string operator()() const&&;

    ~AwesomeCallback() = default;

private:
    std::string s_;
};