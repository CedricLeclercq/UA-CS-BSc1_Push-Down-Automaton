//
// Created by Cédric Leclercq on 01/11/2021.
//

#ifndef PUSH_DOWN_AUTOMATON_STATE_H
#define PUSH_DOWN_AUTOMATON_STATE_H

#include <string>
#include <vector>
#include "Transition.h"


class State {
private:
    std::string name; // Name of the state
    bool starting = false; // Is the state starting
    bool accepting = false; // Is the state accepting
    std::vector<Transition*> transitions; // Transitions from this state
    std::vector<Transition*> origins;

public:
    State(std::string n, bool s, bool a) {
        name = move(n);
        starting = s;
        accepting = a;
    }

    explicit State(std::string n) {
        name = move(n);
    }

    // General setters
    void setName(std::string nName);
    void setStarting(bool nStarting);
    void setAccepting(bool nAccepting);

    // Transition functions
    void addTransition(Transition * nTransition);
    void clearTransitions();

    // Origins function
    void addOrigin(Transition * nTransition);
    void clearTransition();

    // General getters
    std::string getName() const {return this->name; }
    bool getStarting() const {return this->starting; }
    bool getAccepting() const {return this->accepting; }
    std::vector<Transition*> getTransitions() const {return this->transitions; }
    std::vector<Transition*> getOrigins() const {return this->origins; }
};


#endif //PUSH_DOWN_AUTOMATON_STATE_H
