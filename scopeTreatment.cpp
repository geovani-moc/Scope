#include "scopeTreatment.hpp"

void scopeTreatment(vector<vector<Token>> &parserTree)
{
    int root = (int)parserTree.size() -1;
    vector<map<string, void*>> symbolTable;
    scopeTreatment(parserTree, root, symbolTable);
}

void scopeTreatment(vector<vector<Token>> &parserTree, int root, vector<map<string, void*>> &symbolTable)
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
        }
        else if (scopeStart())
        {
            newScope();
            position++;
        }
        else if(declarationStart())
        {
            allocateSymbol();
            return;
        }
        else if(assignmentStart())
        {
            //falta mais coisas nesse
            performOperation();
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
    return true;
}

bool scopeStart()
{
    return true;
}

bool declarationStart()
{
    return true;
}

bool assignmentStart()
{
    return true;
}

void newScope()
{

}

void allocateSymbol()
{

}

void performOperation()
{

}