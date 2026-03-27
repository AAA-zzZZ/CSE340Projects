//------------------------------------
//  g++ --std=c++17 -g Grammar.cpp ParseTable.cpp main.cpp -o program
//------------------------------------

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include "ParseTable.h"
#include "Grammar.h"
using namespace std;

class TableDrivenPredictiveParser
{

public:
    TableDrivenPredictiveParser(const Grammar& grammar,
                                const ParseTable& table,
                                const string& input);
};

TableDrivenPredictiveParser::TableDrivenPredictiveParser(const Grammar& grammar,
                                const ParseTable& table,
                                const string& input)
{
    stack<char> s;
    s.push('$');    s.push(grammar.GetStartVar());
    int pos=0;
    char X = s.top();
    char lookhead;
    bool error = false;
    while (X!='$')
    {
        if(pos<=input.size())   lookhead= input.at(pos);
        else lookhead = '$';
        if(grammar.GetTerminals().count(X)!=0)
        {
            if(X==lookhead)
            {
                s.pop();
                // match(lookahead) which is consume this char
                pos++;
            }
            else    
            {
                error=true;
                break;   
            }
        }
        else{
            int productionNum = table.LookUp(X,lookhead);
            if (productionNum==-1)
            {
                error=true;
                break;
            }    
            Production prod = grammar.GetRules().at(productionNum);
            s.pop();
            for (int i = prod.RightHandSide.size()-1; i>=0; i--)
                s.push(prod.RightHandSide.at(i).name);
        }
        X=s.top();
        pos++;
    }  
};


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
        cfg.AddProduction(numOfProductions,lhs, rhs);
        numOfProductions++;
    }
}