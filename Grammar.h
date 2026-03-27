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
    int order = 1;
    char LeftHandSide;
    vector< Symbol > RightHandSide;
};

//---------------------------------------------------------------------------------
// class Grammar
// (V, Σ, R, S)
//---------------------------------------------------------------------------------
class Grammar
{
public:
    Grammar(){}
    void AddProduction(int order, char leftHand, vector<Symbol>rightHand);
    
    const vector< Production > GetRules()const{return rules;}
    const set<char> GetTerminals()const{ return terminals;}
    const set<char> GetVariables()const{return variables;}
    char GetStartVar()const{return startSymbol;}

    //helper debugging functions
    set<char> GetNullables(){return nullables;}
    map< char, set<char> > GetFIRSTSets()const{return FIRST;}
    map< char, set<char> > GetFOLLOWSets()const{return FOLLOW;}
    map< int, set<char> > GetPREDICTSets()const{return PREDICT;}

    void ComputeNullable();
    void ComputeFirst();
    void ComputeFollow();
    void ComputePredict();


private:
    set< char > variables;
    set< char > terminals;
    vector< Production > rules;

    set<char> nullables;
    map< char, set<char>> FIRST;
    map< char, set<char>> FOLLOW;
    map< int, set<char>> PREDICT;

    char startSymbol;
    char epsilon = '_';

    void AddVariables(char var)
        { variables.insert(var); }
    void AddTerminals(char term)
        { terminals.insert(term); }
    void SetStartVar(char S)
        { startSymbol= S; }
};

#endif
