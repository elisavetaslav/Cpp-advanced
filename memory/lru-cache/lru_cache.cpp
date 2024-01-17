#include "lru_cache.h"

#include <iterator>
#include <list>
#include <string>
#include <unordered_map>

LruCache::LruCache(size_t max_size) {
    max_size_ = max_size;
}

void LruCache::Set(const std::string& key, const std::string& value) {
    if (!map_.count(key)) {
        list_.push_front({key, value});
        map_[key] = list_.begin();
        if (map_.size() > max_size_) {
            map_.erase(list_.back().first);
            list_.pop_back();
        }
    } else {
        list_.erase(map_[key]);
        list_.push_front({key, value});
        map_[key] = list_.begin();
    }
}

bool LruCache::Get(const std::string& key, std::string* value) {
    if (!map_.count(key)) {
        return false;
    }
    list_.push_front({key, (*map_[key]).second});
    list_.erase(map_[key]);
    map_[key] = list_.begin();
    *value = (*map_[key]).second;
    return true;
}