#ifndef TRANSITION_HPP
#define TRANSITION_HPP

#include <vector>
#include <fstream>
#include "token.hpp"

#define GO_TO ('g')
#define SHIFT ('s')
#define REDUCE ('r')

using namespace std;

typedef struct Transition
{
    int state;
    string token;
    char action;
    int actionNumber;
} Transition;

typedef struct Operation
{
    char action;
    int actionNumber;
}Operation;

vector<Transition> readTransitions(const char *fileName);
Operation seekTransition(vector<Transition> &transitions, Token token, int currentState);
void printTransition(vector<Transition> &transitions);

#endif