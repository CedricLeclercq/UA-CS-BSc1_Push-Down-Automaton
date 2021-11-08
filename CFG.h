//
// Created by Cédric Leclercq on 08/10/2021.
//

#ifndef GENERAL_CFG_CFG_H
#define GENERAL_CFG_CFG_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;


class CFG {
protected:

    // Member variables
    std::vector<std::string> variables;
    std::vector<std::string> terminals;
    std::vector<std::pair<std::string, std::vector<std::string>>> productions;
    std::vector<std::vector<std::vector<std::string>>> table;
    std::string start;

    // Member functions
    void createExampleCFG();

public:
    CFG() {
        // According to the assignment, the default constructor should create the given CFG in the assignment
        this->createExampleCFG();
    }

    CFG(const std::string& path) {
        std::ifstream input(path);
        json j;
        input >> j;
        this->parser(j);
    }

    CFG(const std::vector<std::string>& newVariables, const std::vector<std::string>& newTerminals,
        const std::vector<std::pair<std::string, std::vector<std::string>>>& newProductions, const std::string& newStart) {
       // This is a constructor to create any CFG
       this->variables = newVariables;
       this->terminals = newTerminals;
       this->productions = newProductions;
       this->start = newStart;
    }

    void print();
    void parser(const json& j);
    bool accepts(const std::string& str);
    void createTable(const std::string& str);
    static std::vector<std::string> splitInPieces(const std::string& str, int amount);
    void printTable();
    std::vector<std::string> findCharInProd(const std::string& ch);
    std::vector<std::vector<std::string>> cartesianProduct(std::vector<std::vector<std::string>> v);
    bool evaluateAcceptanceTable();
};


#endif //GENERAL_CFG_CFG_H