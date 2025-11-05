#ifndef FILE_BPT_H
#define FILE_BPT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <map>
#include <set>

// File-based B+ Tree implementation
class FileBPlusTree {
private:
    std::string filename;

    // For simplicity, we'll use an in-memory structure that we persist to file
    // A true B+ Tree would have proper disk-based node management
    std::map<std::string, std::set<int>> data;

    void loadFromFile() {
        std::ifstream file(filename, std::ios::binary);
        if (!file) return;

        data.clear();
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        for (size_t i = 0; i < size; i++) {
            // Read key
            size_t key_len;
            file.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
            std::string key(key_len, '\0');
            file.read(&key[0], key_len);

            // Read values
            size_t value_count;
            file.read(reinterpret_cast<char*>(&value_count), sizeof(value_count));
            std::set<int> values;
            for (size_t j = 0; j < value_count; j++) {
                int value;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                values.insert(value);
            }

            data[key] = values;
        }
        file.close();
    }

    void saveToFile() {
        std::ofstream file(filename, std::ios::binary);
        if (!file) return;

        size_t size = data.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (const auto& [key, values] : data) {
            // Write key
            size_t key_len = key.size();
            file.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
            file.write(key.c_str(), key_len);

            // Write values
            size_t value_count = values.size();
            file.write(reinterpret_cast<const char*>(&value_count), sizeof(value_count));
            for (int value : values) {
                file.write(reinterpret_cast<const char*>(&value), sizeof(value));
            }
        }
        file.close();
    }

public:
    FileBPlusTree(const std::string& file = "bpt_data.bin") : filename(file) {
        loadFromFile();
    }

    ~FileBPlusTree() {
        saveToFile();
    }

    void insert(const std::string& key, int value) {
        data[key].insert(value);
        saveToFile();
    }

    void remove(const std::string& key, int value) {
        auto it = data.find(key);
        if (it != data.end()) {
            it->second.erase(value);
            if (it->second.empty()) {
                data.erase(it);
            }
            saveToFile();
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

#endif // FILE_BPT_H