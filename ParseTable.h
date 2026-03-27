#ifndef _PARSETABLE_H_
#define _PARSETABLE_H_

#include <string>
#include <set>
#include <vector>
#include <map>

#include "Grammar.h"
using namespace std;

//------------------------------------
// ParseTable where rows are non terminals and columns are terminal symbols
// used to construct table driven parser
//------------------------------------
class ParseTable
{

public:
    ParseTable(const Grammar& cfg);
    int LookUp(char variable, char terminal) const;
    bool IsAmbiguous(){return ambiguous;}

    void PrintTable();

private:
    const Grammar& grammar;
    map<char, map<char, int>> table;
    bool ambiguous = false;
    void Build();
};
#endif