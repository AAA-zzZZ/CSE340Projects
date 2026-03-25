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
            if(production.RightHandSide.empty())
            {    // if it is epsilon production
                //if lhs hasnt already exist in nullable set yet
                if (!nullables.count(production.LeftHandSide))
                {
                    nullables.insert(production.LeftHandSide);
                    change = true;
                }
                continue;
            } 
            //check for if each symbol on the right hand side is nullable
            bool allNullable = true;
            for(Symbol sym: production.RightHandSide)
            {
                //if one variable is not nullable(eg, is terminal or not in nullable set) 
                //then break and end this for loop
                if(sym.isTerminal ||
                    nullables.count(sym.name)==0)
                {   
                    allNullable=false;
                    break;
                }
            }
            if (allNullable)
            {
                if (!nullables.count(production.LeftHandSide))
                {
                nullables.insert(production.LeftHandSide);
                change = true;
                }
            }    
        }
    }
}

//------------------------------------
// compute the FIRST set for each non terminal
//------------------------------------
void Grammar::ComputeFirst()
{
    bool change = true;
    while(change)
    {
        change = false;
        for (Production prod: rules)
        {
            // if it is epsilon production
            if(prod.RightHandSide.empty())
            {
                
            }
        }
        
    }

}

//------------------------------------
//compute the FOLLOW set for each symbol
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
