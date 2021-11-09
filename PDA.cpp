//
// Created by Cédric Leclercq on 01/11/2021.
//

#include "PDA.h"
#include "Utils.h"
#include <iostream> // TODO remove not needed
using namespace std;
using json = nlohmann::json;

bool comparePairWords2(const pair<string,vector<string>>& a, const pair<string,vector<string>>& b) {
    if (a.first == b.first) {
        string one;
        for (const auto& elem: a.second)
            one += elem;
        string two;
        for (const auto& elem: b.second)
            two += elem;
        return one < two;
    }
    return a.first < b.first;
}

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

CFG PDA::toCFG() {
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
    // Looping over all transitions
    for (auto tr: this->allTransitions) {
        for (auto state: this->states) {
            vector<string> var = {tr->getFrom()->getName(), tr->getStackTop(), state->getName()};
            // The vector pro will be used for everything after the -> ...
            vector<string> pro;
            // No replacements, we have epsilon
            if (tr->getReplacement().empty() and state == tr->getTo()) {
                pro = {tr->getInput()};
                if (find(productions.begin(),productions.end(),
                         make_pair(Utils::vectorToBracketsString(var),pro)) == productions.end()) {
                    productions.emplace_back(Utils::vectorToBracketsString(var),pro);
                }
                break;
            } else {
                //productions.emplace_back(Utils::vectorToBracketsString(var), pro);
                int initialStateSelect = 0;
                for (auto itB = 0; itB < tr->getReplacement().size(); itB++) {
                    if (initialStateSelect >= this->states.size()) {
                        initialStateSelect = 0;
                    }
                    int stateSelect = initialStateSelect;
                    pro = {tr->getInput()};
                    State * prev = tr->getTo();
                    for (auto itA = 0; itA < tr->getReplacement().size(); itA++) {
                        // Doing this for selecting states, but preventing selecting states that don't exist
                        if (stateSelect >= this->states.size())
                            stateSelect = 0;
                        vector<string> proElem;
                        if (itA + 1 == tr->getReplacement().size())
                            proElem = {prev->getName(), tr->getReplacement()[itA], state->getName()};
                        else proElem = {prev->getName(), tr->getReplacement()[itA], this->states[stateSelect]->getName()};
                        prev = this->states[stateSelect];
                        pro.emplace_back(Utils::vectorToBracketsString(proElem));
                        stateSelect++;
                    }
                    initialStateSelect++;
                    productions.emplace_back(Utils::vectorToBracketsString(var), pro);
                }

                /*
                for (auto toGo: this->states) {
                    pro = {tr->getInput()};
                    for (auto rep: tr->getReplacement()) {
                        vector<string> proElem = {tr->getTo()->getName(),tr->getStackTop(),toGo->getName()};
                        pro.emplace_back(Utils::vectorToBracketsString(proElem));
                    }
                    productions.emplace_back(Utils::vectorToBracketsString(var),pro);
                }
                // Now making the more fancy productions
                 */
                /*
                 * At this point, we have our var, so the first part of the -> ...
                 * We only need to calc the second part
                 */

                // TODO productions.emplace_back(...)
            }
        }
    }
    sort(productions.begin(),productions.end(), comparePairWords2); // TODO change that the empty string is last

    /*
    for (auto tr: this->allTransitions) {
        State * prev = tr->getTo(); // Previously used state, needed for the algorithm (see later)
        if (not tr->getReplacement().empty()) {
            for (auto state: this->states) { // We need to iterate over all the (q,x,**STATES**) and create all the productions
                vector<string> var;
                if (state == this->states.front()) {
                    var = {tr->getFrom()->getName(), tr->getStackTop(), tr->getTo()->getName()}; // variable
                    prev = tr->getTo();
                } else {
                    var = {prev->getName(), tr->getStackTop(), state->getName()}; // variable
                    prev = state;
                }
                // everything after the var -> ... . Starts with its input symbol (as it should)
                vector<string> pro = {tr->getInput()};
                productions.emplace_back(Utils::vectorToBracketsString(var), pro);
            }
        } else {
            cerr << "here" << endl;
        }
    }
    */

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

    //auto * cfg = new CFG(variables,terminals,productions,"S");
    CFG cfg(variables,terminals,productions,"S");
    return cfg;
}


