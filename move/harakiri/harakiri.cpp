#include "harakiri.h"
#include <string>

AwesomeCallback::AwesomeCallback(std::string&& s) {
    s_ = std::move(s);
}

std::string AwesomeCallback::operator()() const&& {
    const std::string& s = s_ + "awesomeness";
    delete this;
    return s;
}