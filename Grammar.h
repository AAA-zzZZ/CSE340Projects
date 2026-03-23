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
    Grammar();
    Grammar(set<char> var, set<char> term, vector<Production> rule);

    void SetStartVar(char S);
    void AddProduction(char variable, vector<Production> &righthandside);

    //getters for grammar constructing access
    

private:
    set<char> variables;
    set<char> terminals;
    vector<Production> rules;
    char startSymbol;
};



#endif
