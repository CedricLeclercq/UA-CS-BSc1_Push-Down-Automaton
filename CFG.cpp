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
        /*
        for (const auto& replacement: element.second) {
            cout << replacement;
            if (replacement != element.second.back()) {
                cout << " ";
            } // Add the comma except at the end
            else {
                cout << "`" << endl; // Add the end of the vector to the end
            }
        }
        */
        bool other = false;
        for (auto it = element.second.begin(); it != element.second.end(); it++) {
            cout << (*it);
            if (next(it) != element.second.end()) {
                cout << " ";
            } else cout << "`" << endl;
        }
    } cout << "}" << endl;

    // Printing start
    cout << "S = " << this->start << endl; // TODO maybe remove endl
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

void CFG::createTable(const string& str) {
    this->table.clear(); // Clearing past entries
    this->table.reserve((int)str.size());
    for (int i = 0; i < (int)str.size(); i++) {
        vector<vector<string>> cur; // Current vector to be pushed
        cur.reserve((int)str.size()); // Reserving size
        for (int j = 0; j < (int)str.size(); j++) {
            vector<string> cur2;
            cur.emplace_back(cur2);
        }
        table.push_back(cur);
    }
}

bool CFG::accepts(const string& str) {
    // First creating CYK table
    this->createTable(str);
    // Now setting up the algorithm
    vector<pair<string,vector<string>>> allResults;

    for (int i = 1; i <= (int)str.size(); i++) {
        vector<string> test = CFG::splitInPieces(str,i);
        for (int j = 0; j < (int)test.size(); j++) {
            string toTest = test[j]; // what we should use for calculating

            if (i == 3 and j == 0) {
                cout << "";
            }

            // base case, we are on the first row
            if (i == 1) {
                this->table[i-1][j] = this->findCharInProd(toTest);
                allResults.emplace_back(toTest,this->table[i-1][j]);
            }

            // Normal case (where we are NOT at the first row)
            else if (toTest.size() == 2){
                // TODO if the input is longer than 2 we should divide the input into the prev. findings - FIRST SEE IF THIS IS REALLY NEEDED
                // First calculating cartesian product of what we need
                vector<vector<string>> allComb; // TODO check if this is even correct
                for (auto ch: toTest) {
                    allComb.push_back(this->findCharInProd(string(1,ch)));
                }

                // We assume that cartProd is the correct cartProduct we need
                vector<vector<string>> cartProd = cartesianProduct(allComb);
                // Assuming everything is parsed perfectly before here // TODO
                vector<string> matches;
                for (const auto& item: cartProd) {
                    string str1;
                    for (const auto& item1: item)
                        str1 += item1;
                    for (const auto& prod: this->productions) {
                        string str2;
                        for (const auto& item2: prod.second) {
                            str2 += item2;
                        }
                        if (str1 == str2) {
                            matches.emplace_back(prod.first);
                        }
                    }
                }

                // We found all the matches, we can store it into the table
                this->table[i-1][j] = matches;
                allResults.emplace_back(toTest,this->table[i-1][j]);
                //this->printTable();



            } else {
                // First splitting input, we just need to move the comma
                for (int it = 1; it < toTest.size(); it++) {
                    string sub1 = toTest.substr(0,it);
                    string sub2 = toTest.substr(it,string::npos);
                    cout << toTest << " " << sub1  << " " << sub2 << endl; // TODO remove debug code


                    // The input is now split, we can search for its substrings in our allResults vector
                    pair<vector<string>,vector<string>> found;
                    for (const auto& result: allResults) {
                        if (result.first == sub1) {
                            found.first = result.second;
                        }
                        if (result.first == sub2) {
                            found.second = result.second;
                        }
                    }

                    // We have the two vectors that we need to calculate the cartesian product of
                    vector<vector<string>> cartProd = cartesianProduct({found.first,found.second});

                    vector<string> matches;
                    for (const auto& item: cartProd) {
                        string str1;
                        for (const auto& item1: item)
                            str1 += item1;
                        for (const auto& prod: this->productions) {
                            string str2;
                            for (const auto& item2: prod.second) {
                                str2 += item2;
                            }
                            if (str1 == str2) {
                                matches.emplace_back(prod.first);
                            }
                        }
                    }

                    // We found all the matches, we can store it into the table
                    for (const auto& id: matches) {
                        if (find(this->table[i-1][j].begin(), this->table[i-1][j].end(), id) == this->table[i-1][j].end()) {
                            this->table[i - 1][j].push_back(id);
                        }

                    }
                    //this->printTable();
                }
                allResults.emplace_back(toTest,this->table[i-1][j]);
                sort(this->table[i-1][j].begin(),this->table[i-1][j].end(), compareWords);
            }
        }
    }
    this->printTable();
    return false;
}

vector<string> CFG::splitInPieces(const string& str, int amount) {
    vector<string> ret;
    ret.reserve((int)str.size());
    for (int i = 0; i < (int)str.size(); i++) {
        if (amount <= (int)str.size()) {
            string result = str.substr(i, amount);
            if (result.size() < amount)
                continue;
            else ret.push_back(result);
        }
    }
    return ret;
}

void CFG::printTable() {
    // First finding the longest element for printing the spaces before the |
    int max = 0;
    for (const auto& row: this->table) {
        for (const auto& elem: row) {
            if ((int)elem.size() > max) {
                max = (int)elem.size();
            }
        }
    }
    // Per element in de vector dat een vector minder heeft moet het twee spaties extra toevoegen
    for (int i = (int)this->table.size() - 1; i >= 0; i--) {
        cout << "|";
        int k = 0;
        for (auto it2 = this->table[i].begin(); it2 != this->table[i].end(); it2++) {
            k++;
            string s = " {";
            for (auto it = (*it2).begin(); it != (*it2).end(); it++) {
                s+= (*it);
                if (next(it) != (*it2).end()) {
                    s += ", ";
                } else {
                    string spaces;
                    for (int j = 0; j < max - (*it2).size(); j++) {
                        spaces += "   ";
                    } if (!(*it2).empty() and max != (*it2).size()) {
                        spaces += "  ";
                    }
                    if (max == (*it2).size())
                        spaces += "  ";
                    cout << s + "}" << spaces << "|";
                }
            }
            if (k < this->table.size() - i and (*it2).empty()) {
                string spaces;
                for (int j = 0; j < max - (*it2).size(); j++) {
                    spaces += "   ";
                } if (!(*it2).empty() and max != (*it2).size()) {
                    spaces += "  ";
                }
                if (max == (*it2).size())
                    spaces += "  ";
                cout << s << "}" << spaces << "|";
            }

        } cout << endl;
        /*
        for (auto & j : this->table[i]) { // Or j == size() see for later
            string s = " {";
            for (auto it = j.begin(); it != j.end(); it++) {
                s += (*it);
                if (next(it) != j.end()) {
                    s += ", ";
                } else cout << s + "}  |";
            }
            if (j.empty())
                cout << s << "}  |";
        } cout << endl;
         */
    }
}

std::vector<std::string> CFG::findCharInProd(const std::string& ch) {
    vector<string> result;
    for (const auto& prod: this->productions) {
        for (const auto& rep: prod.second) {
            if (rep == ch) {
                result.push_back(prod.first);
            }
        }
    }
    return result;
}

std::vector<std::vector<std::string>> CFG::cartesianProduct(std::vector<std::vector<std::string>> v) {
    vector<vector<string>> fullResult;
    for (const auto& item: v[0]) {
        for (const auto& item2: v[1]) {
            vector<string> result = {item,item2};
            fullResult.push_back(result);
        }
    }
    return fullResult;
}




