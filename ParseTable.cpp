//------------------------------------
// to compile: g++ --std=c++17 -g Grammar.cpp main.cpp -o debug
//------------------------------------

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include "ParseTable.h"

using namespace std;

ParseTable::ParseTable(const Grammar& g) : grammar(g)
{
    ambiguous=false;
    Build();
}

//------------------------------------
// Construct the ParseTable
//------------------------------------

void ParseTable::Build()
{
    const vector<Production>& rules = grammar.GetRules();
    const map<int, set<char>>& predictSet = grammar.GetPREDICTSets();

    for (int i = 0; i < rules.size(); i++)
    {
        char left = rules[i].LeftHandSide;
        for (char c: predictSet.at(i))
        {
            //if it already has a rule(conflict) then its ambiguous
            if(table[left].count(c)!=0)
                ambiguous=true;
            else
                table[left][c]=i;
        }
    }
}

int ParseTable::LookUp(char variable, char terminal) const
{
    if(table.count(variable)!=0 &&
        table.at(variable).count(terminal)!=0)
        return table.at(variable).count(terminal);
    return -1;
}

void ParseTable::PrintTable()
{
    
}
