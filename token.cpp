#include "token.hpp"

vector<Token> readTokens(const char *fileName)
{
    vector<Token> tokens;

    ifstream file(fileName);
    string buffer;
    while (true)
    {
        getline(file, buffer);
        if (file.eof())
            break;

        tokens.push_back(stringToToken(buffer));
    }

    file.close();
    return tokens;
}

Token stringToToken(string &line)
{
    Token token;

    istringstream tokenStream(line);
    getline(tokenStream, token.symbol, ' ');
    if (!tokenStream.eof())
    {
        getline(tokenStream, token.content);
    }
    token.currentState = -1;
    token.nonTerminal = false;

    return token;
}

void printToken(vector<Token> &tokens)
{
    int size = (int)tokens.size();
    for (int i = 0; i < size; i++)
    {
        printf("[%6s] [%6s] [%4d]\n", tokens[i].symbol.c_str(), tokens[i].content.c_str(), tokens[i].currentState);
    }
}

void printToken(Token &token)
{
    printf("[%6s] [%6s] [%4d]\n", token.symbol.c_str(), token.content.c_str(), token.currentState);
}