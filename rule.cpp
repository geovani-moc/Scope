#include "rule.hpp"

vector<Rule> readRules(const char *fileName)
{
    vector<Rule> rules;

    ifstream file(fileName);
    string buffer;
    while (true)
    {
        getline(file, buffer);
        if(file.eof())break;
        rules.push_back(stringToRule(buffer));
    }

    file.close();
    return rules;
}

Rule seekRule(int number, vector<Rule> &rules)
{
    Rule rule;
    int size = (int)rules.size();

    for (int i = 0; i < size; i++)
    {
        if (rules[i].number == number)
        {
            rule = rules[i];
            return rule;
        }
    }
    
    fprintf(stderr, "Erro: Regra %d nao encontrada\n", number);
    exit(EXIT_FAILURE);
}

Rule stringToRule(string &line)
{
    Rule rule;
    string buffer;
    istringstream ruleStream(line);

    ruleStream >> rule.number >> buffer;
    ruleStream.ignore();
    rule.symbol = stringToToken(buffer);

    while (getline(ruleStream, buffer, ' '))
    {
        rule.symbolDerivation.push_back(stringToToken(buffer));
    }

    rule.unstackQuantity = (int)rule.symbolDerivation.size();
        
    return rule;
}

void printRule(vector<Rule> &rules)
{
    int size = (int)rules.size();

    for (int i = 0; i < size; i++)
    {
        printf("[%4d] [%4s] ",rules[i].number, rules[i].symbol.symbol.c_str() );

        int auxSize = rules[i].symbolDerivation.size();
        for (int j = 0; j < auxSize; j++)
        {
            printf("[%s]",rules[i].symbolDerivation[j].symbol.c_str());
        }
        printf("\n");
    }
    
}