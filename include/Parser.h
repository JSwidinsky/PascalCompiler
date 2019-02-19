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

    /** 
     * Start of the grammar rule definitions 
     * Note that the names of the functionsa are the same as the grammar rules for PL 
     */ 
    void Program();
    void Block();
    void DefinitionPart();
    void Definition();
    void ConstDefinition();
    void VariableDefinition();
    void VariableDefinitionPrime();
    void TypeSymbol();
    void VariableList();
    void ProcedureDefinition();
    //void StatementPart();
    //void Statement();
    //void EmptyStatement();
    //void ReadStatement();
    //void VariableAccessList();
    //void WriteStatement();
    //void ExpressionList();
    //void AssignmentStatement();
    //void ProcedureStatement();
    //void IfStatement();
    //void DoStatement();
    //void GuardedCommandList();
    //void GuardedCommand();
    //void Expression();
    //void PrimaryOperator();
    //void PrimaryExpression();
    //void RelationalOperator();
    //void SimpleExpression();
    //void AddingOperator();
    //void Term();
    //void MultiplyingOperator();
    //void Factor();
    //void VariableAccess();
    //void IndexedSelector();
    void Constant();
    //void Numeral();
    //void BooleanSymbol();
    //void Name();
    /** End grammar rule definitions */

    Token* LookAheadToken;
    queue<Token*> TokenQueue;
};

#endif  //PARSER_H