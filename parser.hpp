#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include "token.hpp"
#include "rule.hpp"
#include "transition.hpp"

using namespace std;

vector<vector<Token>> parser(vector<Token> &tokens, vector<Rule> &rules, vector<Transition> &transitions);
bool acceptGrammar(int currentState, int position, vector<Token> &stack);

#endif