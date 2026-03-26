//------------------------------------
// to compile: g++ --std=c++17 -g Grammar.cpp main.cpp -o debug
//------------------------------------

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include "Grammar.h"

using namespace std;

//--------------------------------------------------------------
// main
//--------------------------------------------------------------
int main()
{
    Grammar cfg;
    string inputString;
    
    //read grammars
    string line;
    int numOfProductions = 0;
    while (getline(cin, line)) {
        if (line.empty()) break;
        // parse line into production, add to g

        char lhs = line[0];
        string rhs_str = line.substr(3);
        vector<Symbol> rhs;

        if(rhs_str != "_")
        {
            for (char c: rhs_str)
            {
                Symbol sym;
                sym.name = c;
                sym.isTerminal = islower(c);
                rhs.push_back(sym);
                
            }
        }
        numOfProductions++;
        cfg.AddProduction(numOfProductions,lhs, rhs);
    }
    cfg.ComputeNullable();
    cfg.ComputeFirst();
    cfg.ComputeFollow();
    cfg.ComputePredict();

    // temporary debug print in main.cpp
    for (const Production& p : cfg.GetRules()) {
        cout << p.LeftHandSide << " -> ";
        if (p.RightHandSide.empty()) cout << "epsilon";
        else for (const Symbol& s : p.RightHandSide) cout << s.name;
        cout << "\n";
    }
    cout << "Start: " << cfg.GetStartVar() << "\n";
    cout << "Variables: ";
    for (char var : cfg.GetVariables()) cout << var << " ";
    cout << "\nTerminals: ";
    for (char term : cfg.GetTerminals()) cout << term << " ";
    cout << "\nNullables: ";
    for (char c: cfg.GetNullables()) cout << c << " ";
    cout << "\nFIRST SETS: \n";
    for (auto const& set: cfg.GetFIRSTSets())
    {
        cout << set.first<<" : {";
        for (char s: set.second)
            cout<< s << ", ";
        cout<<"} \n";
    }
    cout << "\nFOLLOW SETS: \n";
    for (auto const& set: cfg.GetFOLLOWSets())
    {
        cout << set.first<<" : {";
        for (char s: set.second)
            cout<< s << ", ";
        cout<<"} \n";
    }

    cout << "\nPREDICT SETS: \n";
    for (auto const& set: cfg.GetPREDICTSets())
    {
        cout << set.first<<" : {";
        for (char s: set.second)
            cout<< s << ", ";
        cout<<"} \n";
    }


}