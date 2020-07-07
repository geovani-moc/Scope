#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define NON_STATE (-1)

using namespace std;

typedef struct Token
{
    string symbol;
    string content;
    int currentState;
    bool nonTerminal;

} Token;

vector<Token> readTokens(const char * fileName);
Token stringToToken(string &line);
void printToken(vector<Token> &tokens);
void printToken(Token &token);

#endif