//
// Created by Cédric Leclercq on 01/11/2021.
//

#ifndef PUSH_DOWN_AUTOMATON_PDA_H
#define PUSH_DOWN_AUTOMATON_PDA_H

#include <stack>
#include <fstream>
#include <State.h>
#include "json.hpp"
#include "CFG.h"

class PDA {
protected:
    // General PDA information
    std::vector<State*> states;
    std::vector<std::string> stackAlphabet;
    std::vector<std::string> alphabet;
    std::vector<State*> finalStates;
    std::stack<std::string> stack;
    std::vector<Transition*> allTransitions;
    std::string startStack;

    // States information
    State * startState;
    State * currentState;

    // Functions
    void parser(const nlohmann::json& j);


public:
    PDA()=default;
    explicit PDA(const std::string& path) {
        std::ifstream input(path);
        nlohmann::json j;
        input >> j;
        this->parser(j);
    }
    State * findState(const std::string& name);
    CFG * toCFG();
};


#endif //PUSH_DOWN_AUTOMATON_PDA_H
