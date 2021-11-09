//
// Created by Cédric Leclercq on 08/10/2021.
//
#include "CFG.h"
#include "PDA.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
using namespace std;

int main() {
    //CFG cfg;
    //cfg.print();
    //return 0;

    // inlezen uit file
    //CFG cfg("/Users/cedricleclercq/input.json"); // CHange back to input
    //cfg.print();
    PDA pda("/Users/cedricleclercq/Documents/1 School/2BINF/Machines en berkenbaarheid/projects/Push-Down-Automaton/PDA2.json");
    pda.findState("q");
    auto dd = pda.toCFG();
    dd.print();
    //cfg.accepts("aaabb");
    //cfg.accepts("aaabbb");
    return 0;
}