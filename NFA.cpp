//------------------------------------
// to compile: g++ --std=c++17 NFA.cpp  DFA.cpp parser.cpp lexer.cpp
//------------------------------------
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>

#include "NFA.h"


//---------------------------------------------------------------------------------
// print the NFA
//---------------------------------------------------------------------------------
void NFA::Print() const
{
    cout << "NFA Transitions:\n";
    for (const auto& nfa_row : Ntran)
    {
        cout << nfa_row.first << ":\t";
        for (const auto& transition : nfa_row.second)
        {
            cout << transition.first << ": { ";
            for (int state : transition.second)
            cout << state << " ";
            cout << "} ";
        }
        cout << endl;
    }
    cout << "Initial state: " << init_state << endl;
    cout << "Final States: {";
    for (int final: fin_states)
    {
        cout<<final<<", ";
    }
    cout<<"}"<<endl;
    
    
}


//---------------------------------------------------------------------------------
// functions for Thompsons Construction
//:shiru_cute:
//---------------------------------------------------------------------------------

// usage: this->Union(B)
// Union this NFA with B
void NFA::Union(const NFA & other)
{
    //create new start state s
    //create new accept state t
    //add episolon transition from s to the 2 NFAs
    //same for t
    //merge transitions and alphabets
    //update different starting & finish states

    NFA B = other;
    //set offset
    int offset = this->MaxStateId()+1;
    B.OffsetStates(offset);

    //save start and accept state
    int A_start = this->init_state;
    int A_accept = this->SingleAcceptState();

    int B_start = B.init_state;
    int B_accept = B.SingleAcceptState();

    CombineTransitions(B);

    // create new start/end state, basically max label +1
    // and map the new states by their id(value)
    int newStart = this->MaxStateId()+1;
    int newAccept = newStart+1;


    AddEmptyTransition(newStart, A_start);
    AddEmptyTransition(newStart, B_start);

    AddEmptyTransition(B_accept, newAccept);
    AddEmptyTransition(A_accept, newAccept);
    
    this->init_state=newStart;
    this->fin_states.clear();
    this->fin_states.insert(newAccept);

}


// usage: this->Concat(B)
// Concat this NFA with B

void NFA::Concat(const NFA &other)
{
    //offset other's states
    //addtransition(this.fin_state, '_', other.init_state)
    //set fin_states={other.fin_states}
    //combien transitions

    NFA B = other;
    //set offset
    int offset = this->MaxStateId()+1;
    B.OffsetStates(offset);

    //save start and accept state
    int A_start = this->init_state;
    int B_start = B.init_state;

    //save a copy of the old this.fin_states
    set<int> oldFinals = this->fin_states;

    //for every final state f in A, add an empty transition f --'_'--> B_start
    //new final states = all of B’s final states (after offset)
    for (int Afinal: oldFinals)
    {
        AddEmptyTransition(Afinal, B_start);
    }
    CombineTransitions(B);
    this->fin_states= B.fin_states;

}

// usage: this->Kleene()
// make this = this*
void NFA::Kleene()
{
    //create new start state s
    //create new accept state t
    //addtransitions(s, '_', this.init_state)
    //addtransitions(s, '_', t)
    //addtransitions(this.fin_states, '_', this.init_state)
    //addtransitions(this.init_state, '_', this.fin_state)
    //set init_state = s, fin_states = {t}
    int newStart = this->MaxStateId()+1;
    int newAccept = newStart+1;

    AddEmptyTransition(newStart, this->init_state);
    AddEmptyTransition(newStart, newAccept);
    
    set<int> old_fin_states = fin_states;
    for (int finState : old_fin_states)
    {
        AddEmptyTransition(finState, init_state);
        AddEmptyTransition(finState, newAccept);
    }
    init_state=newStart;
    fin_states={newAccept};

}

// off set the states so the index NFA's states dont overlap
void NFA::OffsetStates(int offset)
{
    //create new Ntran map
    map<int, map<char, set<int>>> newTran;
    for (auto& state: Ntran)
    {
        //stateID will be added by offset
        int stateID = state.first;
        for(auto& transition: state.second)
        { 
            char symbol = transition.first;
            for (int nextStates: transition.second)
            {
                //newTran.at(new state id)[corresponding input symbol].insert(nextStates + offset)
                newTran[(offset + stateID)][symbol].insert(nextStates + offset);
            }   
        }
    }
    Ntran = newTran;
    init_state+=offset;
    //set new finish states
    set<int> new_fin_states;
    for (int fin: fin_states)
    {
        new_fin_states.insert(fin + offset);
    }
    fin_states=new_fin_states;
}


void NFA::AddEmptyTransition(int from, int to)
{
    Ntran[from]['_'].insert(to);
}

// check the maximum id of the state (maxlabel)
int NFA::MaxStateId() const
{
    int maxId = init_state;
    for (int f : fin_states)
        if (f > maxId) 
            maxId = f;

    for (const auto& stateEntry : Ntran)
    {
        int fromState = stateEntry.first;
        if (fromState > maxId) 
            maxId = fromState;

        for (const auto& symbolEntry : stateEntry.second)
        {
            for (int toState : symbolEntry.second)
            {
                if (toState > maxId) maxId = toState;
            }
        }
    }

    return maxId;
}



//add the different transitions from other.Ntran into Ntran
void NFA::CombineTransitions(const NFA &other)
{
    //loop through every state in other NFA
    for(const auto& singleState : other.Ntran)
    {
        int fromState = singleState.first;
        for (const auto& symbolEntry : singleState.second)
        {
            //corresponding input char
            char symbol = symbolEntry.first;
            //next states that singleState can go to based on input symbol
            const set<int>& nextStates = symbolEntry.second;

            for (int toState: nextStates)
            {
                //insert the new transition
                //Ntran[fromState] = a map of transitions fromState have
                //Ntran[fromState][symbol] = a map of next States that fromState can go to based on symbol
                this->Ntran[fromState][symbol].insert(toState);
            }   
        }
    }
    //merge alphabet
    for (char c : other.alphabet)
    {
        this->alphabet.insert(c);
    }
}

// checks if the finish state has exactly 1 state
// return the id if there is, else return -1
int NFA::SingleAcceptState() const
{
    //if there is more than 1 finish state, return -1
    if (fin_states.size()!=1)
        return -1;
    //else return the id of the finish state
    return *fin_states.begin();
}





//---------------------------------------------------------------------------------
// epsilon closure of a set of states in the NFA
//
// a dfs traversal of epsilon transitions
//
//---------------------------------------------------------------------------------
set<int> NFA::EpsilonClosure(const set<int>& states)
{
    stack<int> dfs_stack;
    set<int> epsilon_closure_set = states;
    for (int state : states)
    dfs_stack.push(state);
    while (!dfs_stack.empty())
    {
        int state = dfs_stack.top(); dfs_stack.pop();
        // Check all epsilon transitions from this state
        if (Ntran[state].find('_') != Ntran[state].end())
        for (int next_state : Ntran[state]['_'])
        if (epsilon_closure_set.find(next_state) ==
        epsilon_closure_set.end())
        {
            epsilon_closure_set.insert(next_state);
            dfs_stack.push(next_state);
        }
    }
    return epsilon_closure_set;
}


//---------------------------------------------------------------------------------
// construct DFA using subset construction algorithm
//---------------------------------------------------------------------------------
DFA NFA::NFA2DFA()
{
    /*
    ε-closure(qo) is the only state in Q' and is unmarked
    while (there is an unmarked state R in Q')
    mark R
    for (each input symbol a)
    U = δ′[R, a] = ε-closure(move(R,a))
    if (U not in Q')
    add U as an unmarked state to Q'
    */

    //construct and return DFA
    // just add Dtran?

    
    set <int> dfaInitialState = EpsilonClosure({init_state});
    set<char> dfaAlpha = alphabet;
    dfaAlpha.erase('_');
    DFA dfa(dfaAlpha, dfaInitialState, set<int>{});

    //used to look up if a subset is already in Q'
    map <set<int>, int> subsetToId;

    //retrieve subset by id
    vector <set<int>> idToSubset;

    //keeping track if a subset(state) is marked
    vector <bool> marked;

    //used for marking
    queue<int> unmarkedStates;

    //add initial states to all the above
    idToSubset.push_back(dfaInitialState);
    subsetToId.insert({dfaInitialState, 0});
    marked.push_back(false);
    unmarkedStates.push(0);

    //while (there is an unmarked state R in Q')
    while (!unmarkedStates.empty() && !marked[unmarkedStates.front()])
    {
        //mark R
        int Rid = unmarkedStates.front();
        unmarkedStates.pop();
        marked.at(Rid) = true;
        for(char symbol:alphabet)
        {
            if(symbol == '_') continue;
            /*U = δ′[R, a] = ε-closure(move(R,a))
            if (U not in Q')*/
            set<int> U = EpsilonClosure(
                Move(idToSubset.at(Rid), symbol));
            int Uid;
            if(subsetToId.find(U)==subsetToId.end())
            {
                Uid = idToSubset.size();
                subsetToId[U] = Uid;
                idToSubset.push_back(U);
                marked.push_back(false);
                unmarkedStates.push(Uid);
            }
            else
                Uid = subsetToId.find(U)->second;

            dfa.AddTransition(Rid,Uid,symbol);
        }
    }
    //determine DFA final state
    set<int> dfaFinalState;
    for (int i=0; i<idToSubset.size(); i++)
    {
        set<int> subset = idToSubset.at(i);
        for (int finState: fin_states)
        {
            //count how many accepting state in fin_states
            //does subset contains
            if(subset.count(finState)!=0)
            {
                dfaFinalState.insert(i);
                break;
            }
        }
        //add subset to dfaFinalState
        
    }
    //add the finalstates to the dfa
    dfa.setFinalState(dfaFinalState);
        
    return dfa;
}

NFA NFA::PostfixToNFA(const string& postFix, const set<char> alphabet)
{
    stack<NFA> nfaStack;        // stack for operators
    for (int i = 0; i < postFix.size(); i++)
    {
        char c = postFix[i];

        // if c is a symbol over alphabet
        if (alphabet.count(c))
        {
            NFA currentNFA(alphabet, 0, {1});
            currentNFA.AddTransition(0, {1}, c);
            nfaStack.push(currentNFA);
        }

        //else if c is Union (|)
        else if(c=='|' && nfaStack.size()>=2)
        {
            NFA secondNFA = nfaStack.top();
            nfaStack.pop();
            NFA firstNFA = nfaStack.top();
            nfaStack.pop();
            firstNFA.Union(secondNFA);
            nfaStack.push(firstNFA);
        }
        //else if c is Concat (.)
        else if(c=='.' && nfaStack.size()>=2)
        {
            NFA secondNFA = nfaStack.top();
            nfaStack.pop();
            NFA firstNFA = nfaStack.top();
            nfaStack.pop();
            firstNFA.Concat(secondNFA);
            nfaStack.push(firstNFA);
        }

        //else if c is Kleene (*)
        else if(c=='*' && nfaStack.size()>=1)
        {
            NFA topmostNFA = nfaStack.top();
            nfaStack.pop();
            topmostNFA.Kleene();
            nfaStack.push(topmostNFA);
        }
        
    }
    if(nfaStack.size() != 1)
    {
        cout<<"**DEBUG**: error"<<endl; 
    }    
    NFA resultNFA = nfaStack.top();

    return resultNFA;
}

//---------------------------------------------------------------------------------
// Move from one state to another based on input char
// returns a set of states where the current set of states can go based on the input cahr
//---------------------------------------------------------------------------------
set<int> NFA::Move(set<int> current_states, char c)
{
    //set of next states
    set<int> U;
    for(int singleState: current_states)
    {
        //state is a set of transitions at single State
        // there could be no transitions from the singleState
        auto state = Ntran.find(singleState);
        if (state == Ntran.end()) continue;

        // find the corresponding transitions under symbol c at singleState
        // there could be multiple or none
        auto transition = state->second.find(c);
        if (transition == state->second.end()) continue;

        // transition->second is the set of next states that the current single state can go to under the imput symbol c
        set<int>& next_states = transition->second;
        U.insert(next_states.begin(), next_states.end());
        
    }

    return U;
}