#include <iostream>
#include <string>
#include <vector>
#include "file_bpt.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    FileBPlusTree database;

    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;

        if (command == "insert") {
            string index;
            int value;
            cin >> index >> value;
            database.insert(index, value);
        }
        else if (command == "delete") {
            string index;
            int value;
            cin >> index >> value;
            database.remove(index, value);
        }
        else if (command == "find") {
            string index;
            cin >> index;
            vector<int> values = database.find(index);
            if (values.empty()) {
                cout << "null\n";
            } else {
                bool first = true;
                for (int val : values) {
                    if (!first) cout << " ";
                    cout << val;
                    first = false;
                }
                cout << "\n";
            }
        }
    }

    return 0;
}