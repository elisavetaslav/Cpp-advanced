#pragma once

#include "dungeon.h"

#include <unordered_set>

std::unordered_set<std::string> keys;
std::unordered_set<Room*> vis;

Room* Dfs(Room* now) {
    if (now->IsFinal()) {
        return now;
    }
    vis.insert(now);
    Room* answer = nullptr;
    for (size_t i = 0; i < now->NumKeys(); ++i) {
        keys.insert(now->GetKey(i));
    }
    for (size_t i = 0; i < now->NumDoors(); ++i) {
        Door* door = now->GetDoor(i);
        for (std::string key : keys) {
            if (door->TryOpen(key)) {
                if (vis.count(door->GoThrough()) == 0) {
                    answer = Dfs(door->GoThrough());
                }
            }
            if (answer) {
                break;
            }
        }
    }
    return answer;
}

Room* FindFinalRoom(Room* starting_room) {
    keys.clear();
    vis.clear();
    keys.insert("");
    return Dfs(starting_room);
}