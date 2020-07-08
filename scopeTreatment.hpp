#ifndef SCOPE_TREATMENT_HPP
#define SCOPE_TREATMENT_HPP

#include <vector>
#include <string>
#include <map>
#include "token.hpp"

using namespace std;

void scopeTreatment(vector<vector<Token>> &parserTree);
void scopeTreatment(vector<vector<Token>> &parserTree, int root, vector<map<string, void*>> &symbolTable);
bool endOfTreatment(vector<Token> &position);

bool scopeStart();
bool declarationStart();
bool assignmentStart();
void newScope();
void allocateSymbol();
void performOperation();

#endif