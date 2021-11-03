//
// Created by Cédric Leclercq on 01/11/2021.
//

#include "Transition.h"

#include <utility>

void Transition::setFrom(State *nFrom) {
    this->from = nFrom;
}

void Transition::setTo(State *nTo) {
    this->to = nTo;
}

void Transition::setInput(std::string nInput) {
    this->input = std::move(nInput);
}

void Transition::setStackTop(std::string nStackTop) {
    this->stackTop = std::move(nStackTop);
}

void Transition::addReplacement(const std::string& nReplacement) {
    this->replacement.push_back(nReplacement);
}

void Transition::clearReplacement() {
    this->replacement.clear();
}
