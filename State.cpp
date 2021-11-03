//
// Created by Cédric Leclercq on 01/11/2021.
//

#include "State.h"

#include <utility>
using namespace std;

void State::setName(string nName) {
    this->name = move(nName);
}

void State::setStarting(bool nStarting) {
    this->starting = nStarting;
}

void State::setAccepting(bool nAccepting) {
    this->accepting = nAccepting;
}

void State::addTransition(Transition* nTransition) {
    this->transitions.push_back(nTransition);
}

void State::clearTransitions() {
    this->transitions.clear();
}

void State::addOrigin(Transition *nTransition) {
    this->origins.push_back(nTransition);
}

void State::clearTransition() {
    this->origins.clear();
}
