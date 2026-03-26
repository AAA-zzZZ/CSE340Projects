//------------------------------------
// to compile: g++ --std=c++17 -g Grammar.cpp main.cpp -o program
//------------------------------------
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>

#include "Grammar.h"


void Grammar::AddProduction(int order, char leftHand, vector<Symbol> rightHand)
{
    if(rules.empty())
        SetStartVar(leftHand);
    rules.push_back({order, leftHand, rightHand});
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
    //initialize empty sets
    for (char v : GetVariables())
        FIRST[v] = {};
    bool change = true;
    while(change)
    {
        change = false;
        for (Production prod: rules)
        {
            for(Symbol sym: prod.RightHandSide)
            {
                if(sym.isTerminal)
                {
                    //compute FIRST for all terminals
                    if(FIRST[prod.LeftHandSide].count(sym.name)==0)
                    {
                        FIRST[prod.LeftHandSide].insert(sym.name);
                        change = true;
                    }
                    break;
                }
                else
                {
                    //find FIRST for non terminals
                    for(char c: FIRST[sym.name])
                    {
                        if (FIRST[prod.LeftHandSide].count(c)==0)
                        {
                            //add FIRST(B) to FIRST(A) 
                            FIRST[prod.LeftHandSide].insert(c);
                            change = true;
                        }
                    }
                    //if sym is nullable continue to next variable
                    if (nullables.count(sym.name)==0)  break;
                }
            }
        }      
    }
}

//------------------------------------
//compute the FOLLOW set for each symbol
//------------------------------------
void Grammar::ComputeFollow()
{
 //initialize empty sets
    for (char v : GetVariables())
        FOLLOW[v] = {};
    FOLLOW[startSymbol].insert('$');
    bool change = true;
    while(change)
    {
        change = false;
        for (Production prod: rules)
        {
            //prod:B
            for (int i = 0; i < prod.RightHandSide.size(); i++)
            {
                //current :A
                Symbol current = prod.RightHandSide[i];
                if (current.isTerminal) continue;
                
                bool restAllNullable = true;
                //checking anything to the right of A
                for(int j=i+1; j < prod.RightHandSide.size(); j++)
                {
                    //next: C_i
                    Symbol next = prod.RightHandSide[j];
                    
                    if (next.isTerminal){
                        if(FOLLOW[current.name].count(next.name) == 0)
                        {
                            FOLLOW[current.name].insert(next.name);
                            change = true;
                        }
                        restAllNullable=false;
                        break;
                    }
                    //else then rest are all variables
                    else
                    {
                        //case 4&5: add FIRST[C_i] to FOLLOW[A]
                        //when C_i is nullable, it keeps adding to FOLLOW(A)
                        for (char c: FIRST[next.name])
                        {
                            if (FOLLOW[current.name].count(c)==0) 
                            {
                                FOLLOW[current.name].insert(c);
                                change = true;
                            }
                        }
                        //if a C_i is not nullable, add FIRST(C_i) to FOLLOW(A)
                        //and break
                        if(nullables.count(next.name) == 0)
                        {
                            restAllNullable =false;
                            break;
                        }
                        
                    }
                }
                //after the for loop, if rest of all var to the right of A are nullable will be determined
                //if true then add FOLLOW(B) to FOLLOW(A)
                if (restAllNullable)
                {
                    //loop through FOLLOW(B)
                    for (char c: FOLLOW[prod.LeftHandSide])
                    {
                        if(FOLLOW[current.name].count(c) == 0)
                        {
                            FOLLOW[current.name].insert(c);
                            change=true;
                        }
                    }
                }
            }
        }
    }

}


//------------------------------------
//compute the PREDICT set for each variable
//------------------------------------
void Grammar::ComputePredict()
{
    //initialize empty sets
    for (int i=0; i<rules.size();i++)
        PREDICT[i] = {};
    for (Production prod: rules)
    {
        set<char> predictionForProd;
        bool ifEpsilon = false;
        for (Symbol sym: prod.RightHandSide)
        {
            for(char c: FIRST[sym.name])
                predictionForProd.insert(c);
            if(nullables.count(sym.name)!=0)
                ifEpsilon=true;
        }
        if (ifEpsilon)
        {
            for (Symbol sym:prod.RightHandSide)
            {
                for(char c: FOLLOW[sym.name])
                    predictionForProd.insert(c);
            }
        }
        PREDICT[prod.order]=predictionForProd;
    }
    
}
