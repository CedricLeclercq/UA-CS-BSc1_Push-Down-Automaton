//
// Created by Cédric Leclercq on 01/11/2021.
//

#ifndef PUSH_DOWN_AUTOMATON_UTILS_H
#define PUSH_DOWN_AUTOMATON_UTILS_H
#include <iostream>

#include <string>
#include <vector>

class Utils {
public:
    static std::string vectorToBracketsString(std::vector<std::string> v);
};

std::string Utils::vectorToBracketsString(std::vector<std::string> v) {
    std::string result = "[";
    for (const auto& item: v) {
        result += item + ",";
    }
    result.pop_back();
    result += "]";
    return result;
}


/*
void Utils::removeDoublesOfTriples(std::vector<std::vector<std::string>> &v) {
    for (const auto& item: v) {
        if (item.size() != 3) {
            std::cerr << "A problem occurred while sorting the vector." << std::endl;
            return;
        }
    }
    std::vector<std::vector<std::string>> results;
    for (auto item: v) {
        for (auto result: results) {
            if (item[0] == result[0] and item[1] == result[1] and item[2] == result[2]) {
                continue;
            } else {
                results.push_back(item);
            }
        }
    }
    v.clear();
    for (const auto& item: results) {
        v.push_back(item);
    }
}
 */

#endif //PUSH_DOWN_AUTOMATON_UTILS_H
