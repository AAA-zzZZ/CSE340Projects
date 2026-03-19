#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
using namespace std;

struct TokenDef {
    string name;
    string regex;
};

class Parser {
public:
    Parser(string input);

    //function used to parse in and recognizes different rules
    //parse until #
    bool parse(vector<pair<string,string>>& defs, string& inputString);

private:
    string input;
    int pos;
    
};


#endif


