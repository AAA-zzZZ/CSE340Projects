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

//------------------------------------
// compute nullable non terminals
//------------------------------------
void Grammar::ComputeNullable()
{
    bool change = true;
    while(change)
    {
        change = false;
        for (Production production: rules)
        {
            // if it is epsilon production
            if(production.RightHandSide.empty())
            {
                nullables.insert(production.LeftHandSide);
                change = true;
            }    
            
            //check for if each symbol on the right hand side is nullable
            bool allNullable = true;
            for (Symbol sym: production.RightHandSide)
            {
                if (sym.isTerminal ||
                    nullables.find(sym.name) == nullables.end())
                {
                    //if one variable is not nullable then break and end this for loop
                    allNullable=false;
                    break;
                }
            }
            if (allNullable)
            {
                nullables.insert(production.LeftHandSide);
                change = true;
            }    
        }
    }
}

//------------------------------------
// compute the FIRST set for each variable
//------------------------------------
void Grammar::ComputeFirst()
{
    
}

//------------------------------------
//compute the FOLLOW set for each variable
//------------------------------------
void Grammar::ComputeFollow()
{
}


//------------------------------------
//compute the PREDICT set for each variable
//------------------------------------
void Grammar::ComputePredict()
{
}
