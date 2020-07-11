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

    if (symbolTable.back()[variableName].pointer == NULL)
    {
        fprintf(stderr, "Erro: variavel nao declarada.\n");
        exit(EXIT_FAILURE);
    }

    if (symbolTable.back()[variableName].pointerType == TYPE_INT)
    {
        (*(int *)symbolTable.back()[variableName].pointer) =
            (*(int *)expression(parserTree, positionDerivation, symbolTable, TYPE_INT));
    }
    else
    {
        (*(long long int *)symbolTable.back()[variableName].pointer) =
            (*(long long int *)expression(parserTree, positionDerivation, symbolTable, TYPE_LL_INT));
    }

    // imprimir atribuicao
    if (symbolTable.back()[variableName].pointerType == TYPE_INT)
    {
        printf("%s = %d\n", variableName.c_str(), *(int *)symbolTable.back()[variableName].pointer);
    }
    else
    {
        printf("%s = %lld\n", variableName.c_str(), *(long long int *)symbolTable.back()[variableName].pointer);
    }
}

void *expression(vector<vector<Token>> &parserTree, int position,
                 vector<map<string, Memorizer>> &symbolTable,
                 int pointerType)
{
    vector<Token> derivation = parserTree[position];
    void *value = NULL;

    position = 0;
    int size = (int)derivation.size();
    int derivationPosition;

    switch (size)
    {
    case 1: // NUM , ID, STRING
        if (derivation[position].symbol.compare("NUM") == 0)
        {
            if (pointerType == TYPE_INT)
            {
                value = malloc(sizeof(int));
                (*(int *)value) = stoi(derivation[position].content);
            }
            else
            {
                value = malloc(sizeof(long long int));
                (*(long long int *)value) = stoll(derivation[position].content);
            }
            return value;
        }
        else if (derivation[position].symbol.compare("ID") == 0)
        {
            string variableName = derivation[position].content;
            int pointerType2 = symbolTable.back()[variableName].pointerType;

            if (pointerType == TYPE_INT && pointerType2 == TYPE_INT)
            {
                value = malloc(sizeof(int));
                (*(int *)value) = *(int *)symbolTable.back()[variableName].pointer;
            }
            else if (pointerType == TYPE_INT && pointerType2 == TYPE_LL_INT)
            {
                value = malloc(sizeof(int));
                (*(int *)value) = (int)(*(long long int *)symbolTable.back()[variableName].pointer);
            }
            else if (pointerType == TYPE_LL_INT && pointerType2 == TYPE_INT)
            {
                value = malloc(sizeof(long long int));
                (*(long long int *)value) = (long long int)(*(int *)symbolTable.back()[variableName].pointer);
            }
            else if (pointerType == TYPE_LL_INT && pointerType2 == TYPE_LL_INT)
            {
                value = malloc(sizeof(long long int));
                (*(long long int *)value) = *(long long int *)symbolTable.back()[variableName].pointer;
            }
            else
            {
                fprintf(stderr, "Erro tipo de variavel nao detectado");
                exit(EXIT_FAILURE);
            }

            return value;
        }
        else if (derivation[position].symbol.compare("STRING") == 0)
        {
            fprintf(stderr, "Erro: tentativa de atribuir uma strig a um inteiro.\n");
            exit(EXIT_FAILURE);
        }
        break;

    case 2: // EO
        derivationPosition = stoi(derivation[position].content);
        value = expression(parserTree, derivationPosition, symbolTable, pointerType);

        derivationPosition = stoi(derivation[position + 1].content);
        return operation(parserTree, derivationPosition, symbolTable, value, pointerType);

    case 3: // (E)
        derivationPosition = stoi(derivation[position + 1].content);
        return expression(parserTree, derivationPosition, symbolTable, pointerType);

    default:
        fprintf(stderr, "Erro: expressao nao esperada.\n");
        exit(EXIT_FAILURE);
    }

    return value;
}

void *operation(
    vector<vector<Token>> &parserTree, int position,
    vector<map<string, Memorizer>> &symbolTable,
    void* value, int pointerType)
{
    vector<Token> derivation = parserTree[position];
    char option = derivation[0].symbol[0];
    int derivationPosition = stoi(derivation[1].content);

    void* value2;
    value2 = expression(parserTree, derivationPosition, symbolTable, pointerType); // tem que modificar varible name?

    long long int operating1;
    long long int operating2;

    if (pointerType == TYPE_INT)
    {
        operating1 = (long long int)(*(int *)value);
        operating2 = (long long int)(*(int*) value2);
    }
    else
    {
        operating1 = (*(long long int *)value);
        operating2 = (*(long long int *)value2);
    }

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

    if (pointerType == TYPE_INT)
    {
        (*(int *)(value)) = (int)operating1;
    }
    else
    {
        (*(long long int *)value) = operating1;
    }

    return value;
}