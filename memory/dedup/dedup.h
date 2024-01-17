#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::unordered_map<string, size_t> dict;
    std::vector<std::unique_ptr<string>> out(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        if (!dict.count(*items[i])) {
            out[i] = std::make_unique<string>(*items[i]);
            dict[*items[i]] = i;
        } else {
            out[i] = std::make_unique<string>(*out[dict[*items[i]]]);
        }
    }

    return out;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
        const std::vector<std::unique_ptr<string>>& items) {
    std::unordered_map<string, size_t> dict;
    std::vector<std::shared_ptr<string>> out(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        if (!dict.count(*items[i])) {
            out[i] = std::make_shared<string>(*items[i]);
            dict[*items[i]] = i;
        } else {
            out[i] = out[dict[*items[i]]];
        }
    }

    return out;
}