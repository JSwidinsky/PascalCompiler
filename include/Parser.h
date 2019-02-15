#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include "./Token.h"

class Parser
{
public:
    Parser();

    void BeginParsing();


    void GiveTokenQueue(queue<Token*>& TQ);

private:
    void GetNextToken();

    /** Start of the grammar rule definitions */
    void Program();
    void Block();
    void DefinitionPart();
    void Definition();
    void ConstDefinition();
    void VariableDefinition();
    void ProcedureDefinition();




    Token* LookAheadToken;
    queue<Token*> TokenQueue;
};

#endif  //PARSER_H