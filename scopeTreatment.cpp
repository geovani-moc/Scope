#include "scopeTreatment.hpp"

void scopeTreatment(vector<vector<Token>> &parserTree)
{
    int root = (int)parserTree.size() - 1;
    vector<map<string, Memorizer>> symbolTable;
    scopeTreatment(parserTree, root, symbolTable);
}

void scopeTreatment(vector<vector<Token>> &parserTree, int root,
                    vector<map<string, Memorizer>> &symbolTable)
{
    vector<Token> derivation = parserTree[root];
    int position = 0;

    while (endOfTreatment(derivation, position))
    {
        if (derivation[position].nonTerminal)
        {
            int derivationPosition = stoi(derivation[position].content);
            scopeTreatment(parserTree, derivationPosition, symbolTable);
            position++;
        }
        else if (declarationStart(derivation, position))
        {
            position = insertSymbolInTable(symbolTable, derivation);
        }
        else if (assignmentStart(derivation, position))
        {
            performOperation(parserTree, derivation, position, symbolTable);
            return;
        }
        else if (scopeStart(derivation, position))
        {
            newScope(symbolTable);
            position++;
        }
        else if (scopeEnd(derivation, position))
        {
            symbolTable.pop_back();
            position++;
        }
        else
        {
            fprintf(stderr, "Erro no tratamento de escopo.\n");
            exit(EXIT_FAILURE);
        }
    }
}

bool endOfTreatment(vector<Token> &derivation, int position)
{
    int size = (int)derivation.size();

    return (position < size);
}

bool scopeStart(vector<Token> &derivation, int position)
{
    return derivation[position].symbol.compare("{") == 0;
}

bool declarationStart(vector<Token> &derivation, int position)
{
    return ((derivation[position].symbol.compare("int") == 0) ||
            (derivation[position].symbol.compare("long") == 0));
}

bool assignmentStart(vector<Token> &derivation, int position)
{
    if (position + 1 > (int)derivation.size())
        return false;

    return (derivation[position].symbol.compare("ID") == 0) &&
           (derivation[position + 1].symbol.compare("=") == 0);
}

void newScope(vector<map<string, Memorizer>> &symbolTable)
{
    map<string, Memorizer> newSymbolTable;

    if (symbolTable.size() > 0)
    {
        newSymbolTable = symbolTable.back();
    }
        
    symbolTable.push_back(newSymbolTable);
}

int insertSymbolInTable(vector<map<string, Memorizer>> &symbolTable, vector<Token> &derivation)
{
    int positionID;
    string variableName;

    if (derivation[0].symbol.compare("int") == 0)
    {
        positionID = 1;
        Memorizer temporary;

        temporary.pointerType = TYPE_INT;
        temporary.pointer = malloc(sizeof(int));

        variableName = derivation[positionID].content;
        symbolTable.back()[variableName] = temporary;        
    }
    else if (derivation[0].symbol.compare("long") == 0)
    {
        positionID = 3;
        Memorizer temporary;

        temporary.pointerType = TYPE_LL_INT;
        temporary.pointer = malloc(sizeof(long long int));

        variableName = derivation[positionID].content;
        symbolTable.back()[variableName] = temporary;
    }
    else
    {
        fprintf(stderr, "Erro: nao encontrado o tipo de simbolo.\n");
        exit(EXIT_FAILURE);
    }

    return positionID + 2;
}

bool scopeEnd(vector<Token> &derivation, int position)
{
    return derivation[position].symbol.compare("}") == 0;
}

void performOperation(
    vector<vector<Token>> &parserTree,
    vector<Token> &derivation, int position,
    vector<map<string, Memorizer>> &symbolTable)
{
    string variableName = derivation[position].content;

    if (!derivation[position + 2].nonTerminal)
    {
        fprintf(stderr, "Erro: erro de sintaxe de atribuicao.\n");
        exit(EXIT_FAILURE);
    }
    int positionDerivation = stoi(derivation[position + 2].content);

    symbolTable.back()[variableName] = expression(parserTree, positionDerivation, symbolTable);
}

Memorizer expression(vector<vector<Token>> &parserTree, int position,
                     vector<map<string, Memorizer>> &symbolTable)
{
    Memorizer temporary;
    vector<Token> derivation = parserTree[position];

    position = 0;
    int size = (int)derivation.size();
    int derivationPosition;

    switch (size)
    {
    case 1: // NUM , ID, STRING
        if (derivation[position].symbol.compare("NUM") == 0)
        {
            temporary.pointerType = TYPE_INT;
            void *temporaryInt = malloc(sizeof(int));
            (*(int *)temporaryInt) = stoi(derivation[position].content);
            temporary.pointer = temporaryInt;

            return temporary;
        }
        else if (derivation[position].symbol.compare("ID") == 0)
        {
            string symbol = derivation[position].content;
            temporary = symbolTable.back()[symbol];
            temporary.pointerType = TYPE_INT;

            return temporary;
        }
        else if (derivation[position].symbol.compare("STRING") == 0)
        {
            fprintf(stderr, "Erro: tentativa de atribuir uma strig a um inteiro.\n");
            exit(EXIT_FAILURE);
        }
        break;

    case 2: // EO
        derivationPosition = stoi(derivation[position].content);
        temporary = expression(parserTree, derivationPosition, symbolTable);

        derivationPosition = stoi(derivation[position+1].content);
        return operation(parserTree, derivationPosition, symbolTable, temporary);

    case 3: // (E)
        derivationPosition = stoi(derivation[position+1].content);
        return expression(parserTree, derivationPosition, symbolTable);

    default:
        fprintf(stderr, "Erro: expressao nao esperada.\n");
        exit(EXIT_FAILURE);
    }

    return temporary;
}

Memorizer operation(
    vector<vector<Token>> &parserTree, int position,
    vector<map<string, Memorizer>> &symbolTable, Memorizer temporary)
{
    vector<Token> derivation = parserTree[position];
    char option = derivation[0].symbol[0];
    int derivationPosition = stoi(derivation[1].content);

    Memorizer temporary2 = expression(parserTree, derivationPosition, symbolTable);

    long long int operating1;
    long long int operating2;

    if (temporary.pointerType == TYPE_INT)
        operating1 = (long long int)(*(int *)temporary.pointer);
    else
        operating1 = (*(long long int *)temporary.pointer);

    if (temporary2.pointerType == TYPE_INT)
        operating2 = (long long int)(*(int *)temporary2.pointer);
    else
        operating2 = (*(long long int *)temporary2.pointer);

    switch (option)
    {
    case '%':
        operating1 %= operating2;
        break;
    case '/':
        operating1 /= operating2;
        break;
    case '*':
        operating1 *= operating2;
        break;
    case '+':
        operating1 += operating2;
        break;
    case '-':
        operating1 -= operating2;
        break;
    case '<':
        operating1 = (operating1 < operating2);
        break;
    case '!': //!=
        operating1 = (operating1 != operating2);
    case '=': //==
        operating1 = (operating1 == operating2);
        break;
    case '&': //&&
        operating1 = (operating1 && operating2);
        break;
    case '|': //||
        operating1 = (operating1 || operating2);
        break;

    default:
        fprintf(stderr, "Erro: symbolo da operacao nao identificado.\n");
        exit(EXIT_FAILURE);
    }

    if (temporary.pointerType == TYPE_INT)
    {
        (*(int *)(temporary.pointer)) = (int)operating1;
    }
    else
    {
        (*(long long int *)temporary.pointer) = operating1;
    }

    return temporary;
}