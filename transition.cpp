#include "transition.hpp"

vector<Transition> readTransitions(const char *fileName)
{
    vector<Transition> transition;

    ifstream transitionFile(fileName);
    
    Transition buffer;
    while (true)
    {
        transitionFile  >> buffer.state >> buffer.token >> buffer.action >> buffer.actionNumber;
        if(transitionFile.eof())break;
        transition.push_back(buffer);
    }
    transitionFile.close();
    return transition;
}


void printTransition(vector<Transition> &transitions)
{
    int size = (int) transitions.size();
    for (int i = 0; i < size; i++)
    {
        printf("%d %s %c %d\n", 
        transitions[i].state, transitions[i].token.c_str(), 
        transitions[i].action, transitions[i].actionNumber);
    }
    
}

Operation seekTransition(vector<Transition> &transitions, Token token, int currentState)
{
    Operation operation;
    int size = (int)transitions.size();

    for (int i = 0; i < size; i++)
    {
        if((transitions[i].state == currentState) && 
        (transitions[i].token.compare(token.symbol) == 0))
        {
            operation.action = transitions[i].action;
            operation.actionNumber = transitions[i].actionNumber;

            if (operation.actionNumber == NON_STATE)
            {
                operation.action = GO_TO;
            }
            
            return operation;
        }
    }
    
    fprintf(stderr, "Erro: Transicao com estado %d e simbolo \'%s\' nao encontrada.\n", currentState, token.symbol.c_str());
    exit(EXIT_FAILURE);
}