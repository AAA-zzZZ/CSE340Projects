//------------------------------------
// to compile: g++ --std=c++17 NFA.cpp  DFA.cpp parser.cpp lexer.cpp
//------------------------------------
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include "DFA.h"
#include "NFA.h"
#include "parser.h"

using namespace std;

//--------------------------------------------------------------
// class Token
//--------------------------------------------------------------
class Token 
{
public:
    Token(string t, string v) : type(t), value(v) {};
    Token() : type("EOS"), value("") {};

    string type;
    string value;
};

//--------------------------------------------------------------
// class Lexer
//--------------------------------------------------------------
class Lexer 
{
public:
    Lexer(const string& input);

    void buildFromDefs(const vector<pair<string,string>>& defs);
    Token getInputToken();
    DFA Regex2DFA(const string& regex);
    
private:
    struct Rule {
        string name;
        DFA dfa;
    };
    string input;
    int pos;    //current position in the input string
    set<char> alphabet = {};
    vector<Rule> rules;

};


//---------------------------------------------------------------------
// below is shunting (postfix notation) algorithm

//---------------------------------------------------------------------
// define precedence and associativity of operators
//---------------------------------------------------------------------
int Precedence(char op) 
{
    if (op == '*') return 3;  // highest precedence for Kleene star (*)
    if (op == '.') return 2;  // concatenation (.) has lower precedence than *
    if (op == '|') return 1;  // alternation (|) has the lowest precedence
    return 0;
}

//---------------------------------------------------------------------
// check if a character is an operand (alpha)
//---------------------------------------------------------------------
bool IsOperand(char c) 
{
    return isalpha(c);
}


//---------------------------------------------------------------------
// convert an infix regular expression to postfix
//---------------------------------------------------------------------
string InfixToPostfix(const string& infix) 
{
    stack<char> ops;        // stack for operators
    string postfix = "";    // resulting postfix expression 'queue'
    
    for (int i = 0; i < infix.size(); i++) 
    {
        char c = infix[i];
        
        if (IsOperand(c)) 
        {
            // if c is an operand, put it on the output queue
            postfix += c;
        }
        else if (c == '(') 
        {
            // if it's an opening parenthesis, push it onto the stack
            ops.push(c);
        }
        else if (c == ')') 
        {
            // pop off the stack until matching '(' is found
            while (!ops.empty() && ops.top() != '(') 
            {
                postfix += ops.top();
                ops.pop();
            }
            ops.pop();  // pop the '('
        }
        else if (c == '|') 
        {
            // if it's an alternation operator, pop operators of higher precedence
            while (!ops.empty() && ops.top() != '(' && Precedence(ops.top()) >= Precedence(c)) 
            {
                postfix += ops.top();
                ops.pop();
            }
            ops.push(c);  // push the alternation operator onto the stack
        }
        else if (c == '.') 
        {
            // do same for concatenation (.)
            while (!ops.empty() && ops.top() != '(' && Precedence(ops.top()) >= Precedence(c)) 
            {
                postfix += ops.top();
                ops.pop();
            }
            ops.push(c);  // push the . operator onto the stack
        }
        else if (c == '*') 
        {
            // and for (*)
            ops.push(c);  // push the * operator onto the stack
        }
    }
    
    // pop all remaining operators off the stack
    while (!ops.empty()) 
    {
        postfix += ops.top();
        ops.pop();
    }
    
    return postfix;
}


//--------------------------------------------------------------
// Below are functions for Lexer
//--------------------------------------------------------------

Lexer::Lexer(const string& input) : input(input), pos(0) {};

void Lexer::buildFromDefs(const vector<pair<string,string>>& defs) {
    rules.clear();
    alphabet.clear();

    //build alphabet first
    for(auto& pair : defs)
    {
        for (char c: pair.second)
        {
            if(isalnum(c))
                alphabet.insert(c);
        }
    }

    //check for episolon tokens
    vector<string> epsilonToken;

    //then build dfas
    for (auto& pair : defs) {
        Rule rule;
        rule.name = pair.first;
        rule.dfa = Regex2DFA(pair.second);
        rule.dfa.Reset();
        if (rule.dfa.GetAccepted()) 
        {
            epsilonToken.push_back(rule.name);
        }
        rules.push_back(std::move(rule));
    }

    if (!epsilonToken.empty()) {
    cout << "EPSILON IS NOT A TOKEN";
    for (auto& name : epsilonToken) cout << " " << name;
    cout << "\n";
    exit(0);
    }

}

DFA Lexer::Regex2DFA(const string& regex)
{
    string postfix = InfixToPostfix(regex);
    
    //DEBUGGING
    // cout << "regex: " << regex << "\n";
    // cout << "postfix: " << postfix << "\n";
    NFA nfa = NFA::PostfixToNFA(postfix, alphabet);
    return nfa.NFA2DFA();
}


//--------------------------------------------------------------
// return next token from the input string
// reading inputs
//--------------------------------------------------------------
Token Lexer::getInputToken() 
{
    //---- Skip whitespace
    while (pos < input.size() && isspace(input[pos]))
        pos++;
    //---- check for EOS
    if (pos == input.size())
        return Token("EOS", "");

    // Reset all DFAs
    for (auto& r : rules) r.dfa.Reset();
    
    int len = 0; // current length of the lexeme being matched by the DFA
    int best_len = 0; // length of the longest lexeme accepted by any DFA so far
    int matchingType = -1; // token type of the longest accepted lexeme so far
    
    
    int index = pos;  // starting position of the current lexeme being matched
    while (index < (int)input.size() && !isspace((unsigned char)input[index])) {
        char nextChar = input[index];
        bool success = false;
        
        //move all DFAs with the current char
        for (int k = 0; k < (int)rules.size(); k++) {
            auto& dfa = rules[k].dfa;
            if (dfa.GetStatus() != FAIL) {
                dfa.Move(nextChar);
                if (dfa.GetStatus() != FAIL) success = true;

                if (dfa.GetAccepted()) {
                    int len = (int)dfa.GetAcceptedLexeme().size();
                    if (len > best_len || (len == best_len && k < matchingType)) {
                        best_len = len;
                        matchingType = k;
                    }
                }
            }
        }

        if (!success) break;
        index++;
    }

    if (matchingType == -1) {
            char bad = input[pos];
            pos++;
            return Token("INVALID", string(1, bad));
    }

    string lexeme = input.substr(pos, best_len);
    pos += best_len;
    return Token(rules[matchingType].name, lexeme);
}



//--------------------------------------------------------------
// main
//--------------------------------------------------------------
int main() 
{

    string all, line;
    while (getline(cin, line)) {
        all += line;
        all += '\n';
    }

    Parser p(all);

    vector<pair<string, string>> definitions;
    string inputString;

    if(!p.parse(definitions,inputString))
    {
        cout<<"ERROR"<<endl;
        return 0;
    }
    
    Lexer lexer(inputString);
    lexer.buildFromDefs(definitions);
    Token token;
    while (true)
    {
        token=lexer.getInputToken();
        if (token.type == "EOS")
            break;
        if (token.type == "INVALID")
        {
            cout<< "ERROR"<<endl;
            break;
        }
        cout<<token.type<<", "<< "\"" << token.value<< "\"" <<endl;
        
    }
    
    return 0;
}



