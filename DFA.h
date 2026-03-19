#ifndef DFA_H
#define DFA_H

#include <set>
#include <map>

using namespace std;

enum DFAstatus {START, FAIL, POTENTIAL, ACCEPT};

//---------------------------------------------------------------------------------
// class DFA
// (S, Σ, δ, s0, F)
//---------------------------------------------------------------------------------
class DFA 
{
public:
    DFA() {}
    DFA(set<char> A, set<int> I, set<int> F);
    void Reset();

    void AddTransition( int src, int dst, char sym) {Dtran[src][sym] = dst;} // add transition to the DFA
    void Move( char c);
    bool Simulate(string);
    DFAstatus GetStatus() {return status;} //return current status
    string GetAcceptedLexeme() {return accepted_lexeme;} 
    bool GetAccepted() {return accepted;}
    void Print();
    void setFinalState(set<int> finalStates) {fin_states=finalStates;}
    void addFinalState(int singleFinalState) {fin_states.insert(singleFinalState);}
    

private: 
    map< int, map<char, int> > Dtran;  //at state src, on input sym, go to state dst
    DFAstatus status;  // current status of the DFA
    int current_state;  // current state index
    bool accepted;  // is current DFA accepting
    string lexeme;  // input consumed so far
    string accepted_lexeme;   // input consumed so far when the DFA was last in an accepting state
    
    set<char> alpha;      // set of input symbols in the alphabet
    char epsilon = '_';   // epsilon is represented by '_'
    set<int> init_states; // initial state of the DFA
    set<int> fin_states;  // final states of the DFA
};

#endif
