//
// Created by Cédric Leclercq on 08/10/2021.
//
#include "CFG.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
using namespace std;

int main() {
    //CFG cfg;
    //cfg.print();
    //return 0;

    // inlezen uit file
    CFG cfg("/Users/cedricleclercq/input.json");
    cfg.print();
    return 0;


}