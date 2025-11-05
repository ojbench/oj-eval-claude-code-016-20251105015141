#ifndef BPT_H
#define BPT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <map>
#include <set>

// For now, we'll use an in-memory implementation with STL containers
// A true file-based B+ Tree would require disk I/O operations

class BPlusTree {
private:
    std::map<std::string, std::set<int>> data;

public:
    void insert(const std::string& key, int value) {
        data[key].insert(value);
    }

    void remove(const std::string& key, int value) {
        auto it = data.find(key);
        if (it != data.end()) {
            it->second.erase(value);
            if (it->second.empty()) {
                data.erase(it);
            }
        }
    }

    std::vector<int> find(const std::string& key) const {
        auto it = data.find(key);
        if (it != data.end() && !it->second.empty()) {
            return std::vector<int>(it->second.begin(), it->second.end());
        }
        return {};
    }
};

#endif // BPT_H