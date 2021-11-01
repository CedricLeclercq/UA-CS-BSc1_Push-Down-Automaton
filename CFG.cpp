//
// Created by Cédric Leclercq on 08/10/2021.
//

#include "CFG.h"
#include <iostream>

using namespace std;

bool comparePairWords(const pair<string,vector<string>>& a, const pair<string,vector<string>>& b) {
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

bool compareWords(const string& a, const string& b) {
    return a < b;
}

void CFG::createExampleCFG() {
    // Used by the constructor to create the CFG for the first assignment
    this->variables = {"BINDIGIT", "S"};
    this->terminals = {"0", "1", "a", "b"};
    vector<string> prod1 = {""};
    vector<string> prod2 = {"a","S","b","BINDIGIT"};
    vector<string> prod3 = {"0"};
    vector<string> prod4 = {"1"};
    this->productions.emplace_back("BINDIGIT", prod3);
    this->productions.emplace_back("BINDIGIT", prod4);
    this->productions.emplace_back("S", prod1);
    this->productions.emplace_back("S",prod2);
    this->start = "S";
}
void CFG::print() {
    // Printing variables
    cout << "V = {";
    for (const auto& element: this->variables) {
        cout << element; // Print element
        if (element != this->variables.back()) cout << ", "; // Add the comma except at the end
        else cout << "}" << endl; // Add the end of the vector to the end
    }
    // Printing terminals
    cout << "T = {";
    for (const auto& element: this->terminals) {
        cout << element; // Print element
        if (element != this->terminals.back()) cout << ", "; // Add the comma except at the end
        else cout << "}" << endl; // Add the end of the vector to the end
    }
    // Printing productions
    cout << "P = {" << endl;
    for (const auto& element: this->productions) {
        // Printing all replacements
        cout << "    " << element.first << " -> `";
        for (const auto& replacement: element.second) {
            cout << replacement;
            if (replacement != element.second.back()) cout << " "; // Add the comma except at the end
            else cout << "`" << endl; // Add the end of the vector to the end
        }
    } cout << "}" << endl;

    // Printing start
    cout << "S = " << this->start;
}

void CFG::parser(const json& j) {
    vector<string> variablesIn =  j["Variables"];
    vector<string> terminalsIn = j["Terminals"];
    this->variables = variablesIn;
    this->terminals = terminalsIn;
    for (auto prod: j["Productions"]) {
        if (prod["body"].empty()) {
            vector<string> emptyVector = {""};
            this->productions.emplace_back(prod["head"],emptyVector);
        } else {
            this->productions.emplace_back(prod["head"], prod["body"]);
        }
    }
    sort(this->productions.begin(), this->productions.end(), comparePairWords);
    sort(this->terminals.begin(),this->terminals.end(), compareWords);
    sort(this->variables.begin(),this->variables.end(), compareWords);
    string startChar = j["Start"];
    this->start = startChar;
}


