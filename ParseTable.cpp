//------------------------------------
//  g++ --std=c++17 -g Grammar.cpp ParseTable.cpp main.cpp -o program
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
        return table.at(variable).at(terminal);
    return -1;
}

void ParseTable::PrintTable()
{
    cout << "\nPARSE TABLE:\n";
    cout << "NT\t";
    
    // print all terminals as column headers
    for (char t : grammar.GetTerminals())
        cout << t << "\t";
    cout << "$\t\n";

    // print each row
    for (char nt : grammar.GetVariables()) {
        cout << nt << "\t";
        for (char t : grammar.GetTerminals()) {
            int rule = LookUp(nt, t);
            if (rule == -1) cout << "-\t";
            else cout << rule << "\t";
        }
        // check $ column
        int rule = LookUp(nt, '$');
        if (rule == -1) cout << "-\t";
        else cout << rule << "\t";
        cout << "\n";
    }
}
