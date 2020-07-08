#include "scopeTreatment.hpp"

void scopeTreatment(vector<vector<Token>> &parserTree)
{
    int root = (int)parserTree.size() - 1;
    vector<map<string, Memorizer>> symbolTable;
    scopeTreatment(parserTree, root, symbolTable);
}

void scopeTreatment(vector<vector<Token>> &parserTree, int root, vector<map<string, Memorizer>> &symbolTable)
{
    Token token;
    vector<Token> derivation = parserTree[root];
    int position;

    while (endOfTreatment(derivation, position))
    {
        if (token.nonTerminal)
        {
            int derivationPosition = stoi(token.symbol);
            scopeTreatment(parserTree, derivationPosition, symbolTable);
            position++;
            //return;
        }
        else if (declarationStart(derivation, position))
        {
            position = insertSymbolInTable(symbolTable, derivation);
        }
        else if (assignmentStart(derivation, position))
        {
            //falta mais coisas nesse
            performOperation();
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
            return;
        }
        else
        {
            fprintf(stderr, "Erro no tratamento de escopo.\n");
            exit(EXIT_FAILURE);
        }
    }

    return;
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
    return derivation[position].symbol.compare("ID") == 0;
}

void newScope(vector<map<string, Memorizer>> symbolTable)
{
    map<string, Memorizer> newSymbolTable;
    newSymbolTable = symbolTable.back();
    symbolTable.push_back(newSymbolTable);
}

int insertSymbolInTable(vector<map<string, Memorizer>> symbolTable, vector<Token> &derivation)
{
    int positionID;
    if(derivation[0].symbol.compare("int") == 0)
    {
        positionID = 1;
        Memorizer temporary;

        temporary.pointerType = TYPE_INT;
        temporary.pointer = malloc(sizeof(int));

        symbolTable.back()[derivation[positionID].symbol] = temporary;
    }
    else if(derivation[0].symbol.compare("long") == 0)
    {
        positionID = 3;
        Memorizer temporary;
        
        temporary.pointerType = TYPE_LL_INT;
        temporary.pointer = malloc(sizeof(long long int));

        symbolTable.back()[derivation[positionID].symbol] = temporary;
    }
    else
    {
        fprintf(stderr, "Erro: nao encontrado o tipo de simbolo.\n");
        exit(EXIT_FAILURE);
    }

    return positionID+2; //checar overflow
}

void performOperation()
{
}

bool scopeEnd()
{
}