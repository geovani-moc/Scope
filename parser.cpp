#include "parser.hpp"

vector<vector<Token>> parser(vector<Token> &tokens, vector<Rule> &rules, vector<Transition> &transitions)
{
    int position = 0;
    int currentState = 0;
    vector<Token> stack;

    Operation operation;
    Rule currentRule;
    Token token;

    vector<vector<Token>> parserTree;
    vector<Token> temporary;

    while (!acceptGrammar(currentState, position, tokens))
    {
        token = tokens[position];
        if((!stack.empty()) && (stack.back().currentState == NON_STATE))
        {
            operation = seekTransition(transitions, stack.back(), currentState);
        }else
        {
            operation = seekTransition(transitions, token, currentState);
        }

        switch (operation.action)
        {
        case SHIFT:
            stack.push_back(token);
            position++;
            currentState = operation.actionNumber;
            stack.back().currentState = currentState;
            break;

        case REDUCE:
            temporary.clear();
            currentRule = seekRule(operation.actionNumber, rules);

            for (int i = 0; i < currentRule.unstackQuantity; i++)
            {
                temporary.push_back(stack.back());
                stack.pop_back();
            }

            if(stack.empty())
            {
                currentState = 0;
            }else
            {
                currentState = stack.back().currentState;
            }
            
            stack.push_back(currentRule.symbol);
            stack.back().nonTerminal = true;
            
            parserTree.push_back(temporary);
            stack.back().content = to_string(((int)parserTree.size())-1);

            break;

        case GO_TO:
            currentState = operation.actionNumber;
            stack.back().currentState = currentState;
            
            break;

        default:
            fprintf(stderr, "Erro da gramatica: entrada nao aceita\n");
            exit(EXIT_FAILURE);
        }
    }

    //reducao s->VT$.
    currentRule = seekRule(0, rules);
    temporary.clear();

    for (int i = 0; i < currentRule.unstackQuantity; i++)
    {
        temporary.push_back( stack.back());
        stack.pop_back();
    }
    parserTree.push_back(temporary);

    token = currentRule.symbol;
    token.content = to_string((int)parserTree.size() - 1);
    token.nonTerminal = true;

    temporary.clear();
    temporary.push_back(token);
    parserTree.push_back(temporary);

    return parserTree;
}

bool acceptGrammar(int currentState, int position, vector<Token> &stack)
{
    int acceptState = 0;
    if ((currentState == acceptState) && (stack[position].symbol.compare("$") == 0)) return true;

    return false;
}