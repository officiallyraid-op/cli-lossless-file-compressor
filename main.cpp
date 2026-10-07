#include <iostream>
#include <unordered_map>
#include <fstream>

int main() {
    std::ifstream input("test.txt");
    std::unordered_map<char, int> map;

    if (input.fail()) {
        std::cout << "Error" << std::endl;
    }
    else {
        char c;
        while (input.get(c)) {
            map[c]++;
        }

        for (auto const& [ch, count] : map) {
            std::cout << ch << " : " << count << std::endl;
        }
    }
}