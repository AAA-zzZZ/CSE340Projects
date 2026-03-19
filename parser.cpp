//------------------------------------
// to compile: g++ --std=c++17 NFA.cpp  DFA.cpp parser.cpp lexer.cpp
//------------------------------------
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>


#include "PARSER.H"

//--------------------------------------------------------------
//helper functions for scanning inputs
//--------------------------------------------------------------

//skip whitespace when reading token definitions
static void skipWhiteSpace(const string& s, int& pos) {
    while (pos < (int)s.size() && isspace((unsigned char)s[pos])) pos++;
}

//for checking if rule names are legal
static bool isIdStart(char c) {
    return isalpha((unsigned char)c) || c == '_';
}
static bool isIdChar(char c) {
    return isalnum((unsigned char)c) || c == '_';
}


//for reading rule name
static bool readID(const string& s, int& pos, string& outId) {
    skipWhiteSpace(s, pos);
    if (pos >= (int)s.size() || !isIdStart(s[pos]))
        return false;

    int start = pos++;
    while (pos < (int)s.size() && isIdChar(s[pos])) pos++;
    outId = s.substr(start, pos - start);
    return true;
}


// read regex as raw text until ',' or '#' 
static bool readRegex(const string& s, int& pos, string& outRegex) {
    skipWhiteSpace(s, pos);
    int start = pos;

    while (pos < (int)s.size()) {
        char c = s[pos];
        if (c == ',' || c == '#') break;
        pos++;
    }

    int end = pos;
    while (end > start && isspace((unsigned char)s[end - 1])) end--;
    if (end <= start) 
        return false;

    outRegex = s.substr(start, end - start);
    return true;
}


// reaading the quotation marks so it ensures it separates token definition and actual input char
//that the lexer is trying to tokenize
static bool readQuotedString(const string& s, int& pos, string& outStr) {
    skipWhiteSpace(s, pos);
    if (pos >= (int)s.size() || s[pos] != '"')
    {
        cout<<"You are missing \" at pos " + to_string(pos)+ " lil bro"<<endl;
        return false;
    }
    pos++;

    string out;
    while (pos < (int)s.size()) {
        char c = s[pos++];
        if (c == '"') { outStr = out; return true; }
        out.push_back(c);
    }
    cout<<"You are missing \" at the end lil bro"<<endl;
    return false;
}



Parser::Parser(string input) : input(input), pos(0){}



// the arguments: 
// vector<pair<string, string>> &defs is an array that holds rules
// inputString is just inputString
bool Parser::parse(vector<pair<string, string>> &defs, string &inputString)
{
    defs.clear();
    inputString.clear();

    // read until #
    while (true)
    {
        skipWhiteSpace(input, pos);
        if(pos>= (int) input.size())
        {
            cout<<"You are missing # at the end lil bro"<<endl;
            return false;
        }

        if (input[pos] == '#') {
            pos++; 
            break;
        }

        //read token name and regex
        string name, regex;
        if (!readID(input,pos,name))
        {
            cout<<"Expected token name at pos " + to_string(pos)+" lil bro"<<endl;
            return false;
        }
        if (!readRegex(input,pos,regex))
        {
            cout<<"Empty regex after " +name +" lil bro"<<endl;
            return false;
        }

        defs.push_back({name,regex});

        skipWhiteSpace(input, pos);
        if (pos >= (int)input.size()) {
            cout << "where's the input string lil bro? \n";
            return false;
        }
        if (input[pos] == ',') { pos++; continue; }
        if (input[pos] == '#') { pos++; break; }

        cout << "expected ',' or '#' at pos " << pos << "lil bro\n";
        return false;
    }

    if (!readQuotedString(input, pos, inputString)) {
        cout << "there's no quotation mark after '#' lil bro\n";
        return false;
    }

    skipWhiteSpace(input, pos);
    if (pos != (int)input.size()) {
        cout << "extra characters after quoted string at pos " << pos << "lil bro\n";
        return false;
    }
    return true;
    

}