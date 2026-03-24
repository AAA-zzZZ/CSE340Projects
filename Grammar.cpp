//------------------------------------
// to compile: 
//------------------------------------
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>

#include "Grammar.h"


void Grammar::AddProduction(char leftHand, vector<Symbol> rightHand)
{
    if(rules.empty())
        SetStartVar(leftHand);
    rules.push_back({leftHand, rightHand});
    AddVariables(leftHand);
    for (const Symbol sym: rightHand)
    {
        if (sym.isTerminal) AddTerminals(sym.name);
        else AddVariables(sym.name);
    }
}
