#include <vector>
#include <string>
#include "parser.hpp"
#include "transition.hpp"
#include "rule.hpp"
#include "token.hpp"
#include "scopeTreatment.hpp"

using namespace std;

int main()
{
    const char *fileTokens = "tokens4.txt";
    const char *fileRules = "rules.txt";
    const char *fileTransitions = "transitions.txt";

    Token lastToken = {"$", "", -1};
    vector<Token> tokens = readTokens(fileTokens);
    tokens.push_back(lastToken);
    //printToken(tokens);

    vector<Rule> rules = readRules(fileRules);
    //printRule(rules);
    
    vector<Transition> transitions = readTransitions(fileTransitions);
    //printTransition(transitions);

    vector<vector<Token>> parserTree = parser(tokens, rules, transitions);

    scopeTreatment(parserTree);

    return 0;
}
