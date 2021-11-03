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
    std::vector<std::string> variables; // TODO fix "problem" where here [...] can be given as well when someone wants to do CFG->PDA
    std::vector<std::string> terminals;
    std::vector<std::pair<std::string, std::vector<std::string>>> productions;
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
};


#endif //GENERAL_CFG_CFG_H