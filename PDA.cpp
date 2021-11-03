//
// Created by Cédric Leclercq on 01/11/2021.
//

#include "PDA.h"
#include "Utils.h"
#include <iostream> // TODO remove not needed
using namespace std;
using json = nlohmann::json;

void PDA::parser(const json &j) {
    // Setting new Alphabets
    vector<string> nAlphabet = j["Alphabet"];
    this->alphabet = nAlphabet;
    vector<string> nStackAlphabet = j["StackAlphabet"];
    this->stackAlphabet = nStackAlphabet;

    // Creating new states and assigning them their transitions
    for (const auto& state: j["States"]) {
        auto *newState = new State(state, false, false);
        this->states.push_back(newState);
    }

    // Assigning all the transitions to the states
    for (auto state: this->states) {
        for (auto transition: j["Transitions"]) {
            State * nFrom = findState(transition["from"]);
            State * nTo = findState(transition["to"]);
            if (nFrom == state or nTo == state) {
                string nStackTop = transition["stacktop"];
                string nInput = transition["input"];
                vector<string> nReplacement = transition["replacement"];
                auto *nTransition = new Transition(nFrom, nTo, nInput, nStackTop, nReplacement);
                this->allTransitions.push_back(nTransition);
                if (nFrom == state)
                    state->addTransition(nTransition);
                if (nTo == state)
                    state->addOrigin(nTransition);
            }
        }
    }

    // Setting start state and start stack symbol
    auto * nStartState = this->findState(j["StartState"]);
    nStartState->setStarting(true);
    string nStartStack = j["StartStack"];
    this->startState = nStartState;
    this->currentState = nStartState;
    this->startStack = nStartStack;
}

State * PDA::findState(const string &name) {
    for (auto state: this->states) {
        if (state->getName() == name)
            return state;
    }
    cerr << "A state was searched that does not exist!" << endl;
    cerr << "Problem occurred in: State * PDA::findState(const string &name);" << endl;
    return nullptr;
}

CFG *PDA::toCFG() {
    // Finding all the variables
    vector<vector<string>> variablesVector{};
    for (auto state: this->states) {
        for (const auto& stackSymbol: this->stackAlphabet) {
            for (auto stateOne: this->states) {
                variablesVector.push_back({state->getName(),stackSymbol,stateOne->getName()});
            }
        }
    }

    // Finding all the terminals
    vector<string> terminals = this->alphabet;

    // Defining productions
    vector<pair<string,vector<string>>> productions;

    // Finding replacement rules S
    for (auto var: variablesVector) {
        // Fixing productions for S
        if (this->findState(var[0])->getStarting() and this->startStack == var[1]) {
            vector<string> result = {Utils::vectorToBracketsString(var)};
            productions.emplace_back("S", result);
        }
        // Fixing other productions
    }

    for (auto tr: this->allTransitions) {
        vector<string> var = {tr->getFrom()->getName(),tr->getStackTop(),tr->getTo()->getName()}; // variable
        vector<string> rep = {tr->getInput()};// replacement
        vector<string> trRep = tr->getReplacement(); // transition replacements
        if (trRep.empty())
            continue;
        string lsName = tr->getTo()->getName(); // last used state's name
        for (int i = 0; i < trRep.size() and i < this->states.size(); i++) {
            if (trRep[i] == trRep.back()) {
                string item = Utils::vectorToBracketsString(
                        {this->states[0]->getName(), tr->getStackTop(), tr->getTo()->getName()});
                rep.push_back(item);
            }
        }
        productions.emplace_back(Utils::vectorToBracketsString(var),rep);
    }


    /*
    for (auto transition: this->allTransitions) {
        for (auto state: this->states) {
            vector<string> replacements = transition->getReplacement();
            string temp = "[" + transition->getFrom()->getName() + ","+ transition->getStackTop() + "," + transition->getTo()->getName() + "]";
            vector<string> result = {transition->getInput()};
            for (int i = 0; i < this->states.size() and i < transition->getReplacement().size(); i++) {
                vector<string> vectorReplacement;
                if (replacements[i] != replacements.back()) {
                    vectorReplacement = {transition->getTo()->getName(), replacements[i],
                                                        this->states[i]->getName()};
                } else if (not transition->getReplacement().empty()) {
                    vectorReplacement = {this->states[i]->getName(),replacements.back(),transition->getTo()->getName()};
                }
                string replacement = Utils::vectorToBracketsString(vectorReplacement);
                result.push_back(replacement);

            }
            productions.emplace_back(temp,result);
        }
    }
    */
    // Parsing var vector
    vector<string> variables = {"S"};
    variables.reserve(variablesVector.size());
    for (const auto& item: variablesVector)
        variables.push_back(Utils::vectorToBracketsString(item));

    auto * cfg = new CFG(variables,terminals,productions,"S");
    return cfg;
}


