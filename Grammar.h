#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;


struct Symbol
{
    char name;
    bool isTerminal;
};

struct Production
{
    char LeftHandSide;
    vector<Symbol> RightHandSide;
};

//---------------------------------------------------------------------------------
// class Grammar
// (V, Σ, R, S)
//---------------------------------------------------------------------------------
class Grammar
{
public:
    Grammar(){}
    void AddProduction(char leftHand, vector<Symbol> rightHand);
    
    const vector<Production> GetRules(){return rules;}
    const set<char> GetTerminals(){ return terminals;}
    const set<char> GetVariables(){return variables;}
    const char GetStartVar(){return startSymbol;}


    void ComputeNullable();
    void ComputeFirst();
    void ComputeFollow();
    void ComputePredict();


private:
    set<char> variables;
    set<char> terminals;
    vector<Production> rules;

    set<char> nullables;
    map<char, set<char>> FIRST;
    map<char, set<char>> FOLLOW;

    char startSymbol;
    char epsilon = '_';
    void AddVariables(char var)
        {variables.insert(var);}
    void AddTerminals(char term)
        {terminals.insert(term);}
    void SetStartVar(char S)
        {startSymbol = S;}
};

#endif
