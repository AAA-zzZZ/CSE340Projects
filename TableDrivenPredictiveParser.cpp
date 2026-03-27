//------------------------------------
// to compile:  g++ --std=c++17 -g Grammar.cpp ParseTable.cpp TableDrivenPredictiveParser.cpp
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

    string currentDerivation  ="";
    currentDerivation+=grammar.GetStartVar();
    string output = currentDerivation;

    while (X!='$')
    {
        if(pos<input.size())   lookhead= input.at(pos);
        else lookhead = '$';
        if(grammar.GetTerminals().count(X)!=0)
        {
            if(X==lookhead){ s.pop();
                // match(lookahead) which is consume this char
                pos++;}
            else{error=true; break;}
        }
        else{
            int productionNum = table.LookUp(X,lookhead);
            if (productionNum==-1){error=true; break;}    
            Production prod = grammar.GetRules().at(productionNum);
            s.pop();
            
            string right = "";
            for (Symbol sym:prod.RightHandSide)
                right+=sym.name;
            size_t currentDerivePos = currentDerivation.find(X);
            //output X → Y1Y2...Yk
            if(currentDerivePos!=currentDerivation.npos)
                currentDerivation.replace(currentDerivePos,1,right);
            output+=" "+currentDerivation;
            
            for (int i = prod.RightHandSide.size()-1; i>=0; i--)
                s.push(prod.RightHandSide.at(i).name);
        }
        X=s.top();
    }  

    if (pos<input.size())   error=true;
    if(error)   cout<<"ERROR"<<endl;
    else cout<<output<<endl;
    
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
    //read input string
    string quoted;
    while (getline(cin,quoted))
    {
        if (!quoted.empty())  break;
    }
    inputString = quoted.substr(1,quoted.size()-2);
    //cout<<"*********************8TEST: "<<quoted<<endl;

    //compute
    cfg.ComputeNullable();
    cfg.ComputeFirst();
    cfg.ComputeFollow();
    cfg.ComputePredict();
    ParseTable pt(cfg);

    if(pt.IsAmbiguous())
    {
        cout<<"ERROR"<<endl;
        return 0;
    }    
    //else pt.PrintTable();

    TableDrivenPredictiveParser parser(cfg, pt, inputString);
}