#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include "./Administration.h"
#include "./Token.h"

class Administration;

class Parser
{
public:
    Parser(Administration* A);

    void BeginParsing();

private:
    void GetNextToken();

    /**
     * Matches a given symbol with the look-ahead token; if they agree, the next token is fetched
     * If they do not agree, the function calls a syntax error
     * @param symbol Token symbol we are to compare with the look ahead token
     * FunctionID The integer id of the function who called us; used for error handling in the FOLLOW set of a grammar rule
     */
    void Match(const Symbol::Symbol symbol, int FunctionID);

    /**
     * Checks if a symbol is in the FOLLOW set for a particular non-terminal, which is passed as the function id
     * @param symbol terminal symbol we are checking for membership in the follow set
     * @param FunctionID id of the function that the parser is currently in
     * @return true if symbol is in the FOLLOW set, false otherwise
     */
    bool Member(const Symbol::Symbol symbol, int FunctionID);
    bool Check(const Symbol::Symbol symbol);

    /** 
     * Start of the grammar rule definitions 
     * Note that the names of the functions are the same as the grammar rules for PL 
     * Each of them have an associated ID for proper error handling, which are given as comments besides the function name
     */ 
    void Program();                     //ID: 0
    void Block();                       //ID: 1
    void DefinitionPart();              //ID: 2
    void Definition();                  //ID: 3
    void ConstDefinition();             //ID: 4
    void VariableDefinition();          //ID: 5
    void VariableDefinitionPrime();     //ID: 6
    void TypeSymbol();                  //ID: 7
    void VariableList();                //ID: 8
    void ProcedureDefinition();         //ID: 9
    void StatementPart();               //ID: 10
    void Statement();                   //ID: 11
    void EmptyStatement();              //ID: 12
    void ReadStatement();               //ID: 13
    void VariableAccessList();          //ID: 14
    void WriteStatement();              //ID: 15
    void ExpressionList();              //ID: 16
    void AssignmentStatement();         //ID: 17
    void ProcedureStatement();          //ID: 18
    void IfStatement();                 //ID: 19
    void DoStatement();                 //ID: 20
    void GuardedCommandList();          //ID: 21
    void GuardedCommand();              //ID: 22
    void Expression();                  //ID: 23
    void PrimaryOperator();             //ID: 24
    void PrimaryExpression();           //ID: 25
    void RelationalOperator();          //ID: 26
    void SimpleExpression();            //ID: 27
    void AddingOperator();              //ID: 28
    void Term();                        //ID: 29
    void MultiplyingOperator();         //ID: 30
    void Factor();                      //ID: 31
    void VariableAccess();              //ID: 32
    void IndexedSelector();             //ID: 33
    void Constant();                    //ID: 34
    void Numeral();                     //ID: 35
    void BooleanSymbol();               //ID: 36
    void Name();                        //ID: 37
    /** End grammar rule definitions */

    Administration* Admin;
    Token* LookAheadToken;
};

#endif  //PARSER_H