//
// Created by Cédric Leclercq on 01/11/2021.
//

#ifndef PUSH_DOWN_AUTOMATON_TRANSITION_H
#define PUSH_DOWN_AUTOMATON_TRANSITION_H

class State;

#include <utility>
#include <string>
#include <vector>

class Transition {
protected:
    State * from;
    State * to;
    std::string input;
    std::string stackTop;
    std::vector<std::string> replacement;
public:
    Transition(State * nFrom, State * nTo, std::string nInput, std:: string nStackTop,
               std::vector<std::string> nReplacement) {
        this->from = nFrom;
        this->to = nTo;
        this->input = std::move(nInput);
        this->stackTop = std::move(nStackTop);
        this->replacement = std::move(nReplacement);
    }

    void setFrom(State * nFrom);
    void setTo(State * nTo);
    void setInput(std::string nInput);
    void setStackTop(std::string nStackTop);

    void addReplacement(const std::string& nReplacement);
    void clearReplacement();

    State * getFrom() const {return this->from;}
    State * getTo() const {return this->to;}
    std::string getInput() const {return this->input;}
    std::string getStackTop() const {return this->stackTop;}
    std::vector<std::string> getReplacement() const {return this->replacement;}
};


#endif //PUSH_DOWN_AUTOMATON_TRANSITION_H
