#ifndef SCOPE_TREATMENT_HPP
#define SCOPE_TREATMENT_HPP

#include <vector>
#include <string>
#include <map>
#include "token.hpp"

using namespace std;

void scopeTreatment(vector<vector<Token>> &parserTree);
void scopeTreatment(vector<vector<Token>> &parserTree, int root, vector<map<string, void*>> &symbolTable);

bool endOfTreatment(vector<Token> &derivation, int position);
bool scopeStart(vector<Token> &derivation, int position);
bool declarationStart(vector<Token> &derivation, int position);
bool assignmentStart(vector<Token> &derivation, int position);
bool scopeEnd(vector<Token> &derivation, int position);

void newScope(vector<map<string, void *>> symbolTable);
int insertSymbolInTable(vector<map<string, void *>> symbolTable, vector<Token> &derivation);
void performOperation();

#endif