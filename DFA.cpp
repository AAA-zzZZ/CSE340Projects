//------------------------------------
// to compile: g++ --std=c++17 NFA.cpp  DFA.cpp parser.cpp lexer.cpp
//------------------------------------
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>

#include "DFA.h"


//---------------------------------------------------------------------------------
// DFA actor
// Implement the DFA class's functions
//---------------------------------------------------------------------------------
DFA::DFA(set<char> A, set<int> I, set<int> F) : alpha(A), init_states(I), fin_states(F)
{
    Reset();
}

void DFA::Reset() 
{
    status = START; 
    current_state = *init_states.begin(); 
    accepted=false;
    lexeme.clear();
    accepted_lexeme.clear();
     // ε acceptance: start state is final?
    accepted = (fin_states.count(current_state) > 0);
    if (accepted) {
        status = ACCEPT;
        accepted_lexeme = "";
    }
}

//--------------------------------------------------------------------------------- 
// Move from one state, s, to another based on input char, 
// possibly changing both the current state and the status of the DFS 
//---------------------------------------------------------------------------------

void DFA::Move( char c) 
{ 
    //---- check if c is in the map for transitions
    if (Dtran.at(current_state).find(c) != Dtran.at(current_state).end()) 
    {
        current_state = Dtran.at(current_state)[c];
        lexeme += c;

        if (fin_states.count(current_state) > 0)
        {
            status = ACCEPT;
            accepted = true;
            accepted_lexeme = lexeme;
        }
        else
            status = POTENTIAL;
    }
    else
        status = FAIL;
}



//---------------------------------------------------------------------------------
// print the DFA
//---------------------------------------------------------------------------------
void DFA::Print() 
{
    cout << "DFA Transitions:\n";
    for (const auto& dfa_row : Dtran) 
    {
        cout << dfa_row.first << ":\t";
        for (const auto& transition : dfa_row.second) 
            cout << transition.first << ": " << transition.second << " "; 
        cout << endl;
    }
}

//---------------------------------------------------------------------------------
// simulate the DFA
//
// s = so
// c = nextChar(x) 
// while (c != eof)
//    s = move(s,c)
//    c = nextChar(x) 
// return (s in F)
//---------------------------------------------------------------------------------
bool DFA::Simulate(string input) 
{
    Reset();
    // map< int, map<char, int> > Dtran;
    bool first = true;
    cout << "{";
    for(auto &[k,v] : Dtran)
    {
        if (!first) cout << ",";
        cout << k << ":{";
        bool ffirst = true;
        for(auto &[kk,vv] : v)  
        {
            if (!ffirst) cout << ",";
            cout << kk << ":" << vv; 
            ffirst = false;
        }
        cout << "}";
        first = false;
    }
    cout << "}" << endl;

    int s = *init_states.begin();
    int pos = 0;
    while (pos < input.length())
    {
        char c = input[pos++];
        Move( c );
        //cout << "state: " << s << endl;
    }
    return fin_states.count(current_state) > 0;
}


