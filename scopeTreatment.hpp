#ifndef SCOPE_TREATMENT_HPP
#define SCOPE_TREATMENT_HPP

#include <vector>
#include <string>
#include <map>
#include "token.hpp"

#define TYPE_INT (1)
#define TYPE_LL_INT (2)

using namespace std;

typedef struct Memorizer
{
    void *pointer;
    int pointerType;
} Memorizer;

void scopeTreatment(vector<vector<Token>> &parserTree);
void scopeTreatment(vector<vector<Token>> &parserTree,
                    int root, vector<map<string, Memorizer>> &symbolTable);

bool endOfTreatment(vector<Token> &derivation, int position);
bool scopeStart(vector<Token> &derivation, int position);
bool declarationStart(vector<Token> &derivation, int position);
bool assignmentStart(vector<Token> &derivation, int position);
bool scopeEnd(vector<Token> &derivation, int position);

void newScope(vector<map<string, Memorizer>> &symbolTable);
int insertSymbolInTable(vector<map<string, Memorizer>> &symbolTable, vector<Token> &derivation);
void performOperation(
    vector<vector<Token>> &parserTree,
    vector<Token> &derivation, int position,
    vector<map<string, Memorizer>> &symbolTable);
Memorizer expression(vector<vector<Token>> &parserTree, int position,
                     vector<map<string, Memorizer>> &symbolTable);
Memorizer operation(
    vector<vector<Token>> &parserTree, int position,
    vector<map<string, Memorizer>> &symbolTable,
    Memorizer temporary);
#endif