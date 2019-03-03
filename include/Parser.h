#ifndef PARSER_H
#define PARSER_H

#include <set>
#include "./Administration.h"
#include "./Token.h"

class Administration;

typedef set<Symbol::Symbol> StopSet;

enum GrammarRules 
{
    EProgram, EBlock, EDefinitionPart, EDefinition, EConstantDefinition, EVariableDefinition, EVariableDefinitionPrime,
    ETypeSymbol, EVariableList, EProcedureDefinition, EStatementPart, EStatement, EEmptyStatement, EReadStatement, EVariableAccessList,
    EWriteStatement, EExpressionList, EAssignmentStatement, EProcedureStatement, EIfStatement, EDoStatement, EGuardedCommandList,
    EGuardedCommand, EExpression, EPrimaryOperator, EPrimaryExpression, ERelationalOperator, ESimpleExpression, EAddingOperator, ETerm,
    EMultiplyingOperator, EFactor, EVariableAccess, EIndexedSelector, EConstant, ENumeral, EBooleanSymbol, EName
};

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
     */
    void Match(const Symbol::Symbol symbol, StopSet Sts);

    /**
     * Checks if a symbol is in the FOLLOW set for a particular non-terminal, which is passed as the function id
     * @param symbol terminal symbol we are checking for membership in the follow set
     * @param FunctionID id of the function that the parser is currently in
     * @return true if symbol is in the FOLLOW set, false otherwise
     */
    bool Member(const Symbol::Symbol symbol, StopSet Sts);

    bool Member(const Symbol::Symbol symbol, int FirstSetID);
    bool Check(const Symbol::Symbol symbol);

    StopSet CreateSet(int FirstSetIndex);
    StopSet Union(const StopSet S1, const StopSet S2);
    StopSet Union(const StopSet S, const Symbol::Symbol symbol);


    /** 
     * Start of the grammar rule definitions 
     * Note that the names of the functions are the same as the grammar rules for PL 
     */ 
    void Program(StopSet Sts);                     
    void Block(StopSet Sts);                       
    void DefinitionPart(StopSet Sts);              
    void Definition(StopSet Sts);                  
    void ConstDefinition(StopSet Sts);             
    void VariableDefinition(StopSet Sts);          
    void VariableDefinitionPrime(StopSet Sts);     
    void TypeSymbol(StopSet Sts);                      
    void VariableList(StopSet Sts);                
    void ProcedureDefinition(StopSet Sts);         
    void StatementPart(StopSet Sts);               
    void Statement(StopSet Sts);                   
    void EmptyStatement(StopSet Sts);              
    void ReadStatement(StopSet Sts);               
    void VariableAccessList(StopSet Sts);          
    void WriteStatement(StopSet Sts);              
    void ExpressionList(StopSet Sts);              
    void AssignmentStatement(StopSet Sts);             
    void ProcedureStatement(StopSet Sts);          
    void IfStatement(StopSet Sts);                     
    void DoStatement(StopSet Sts);                 
    void GuardedCommandList(StopSet Sts);          
    void GuardedCommand(StopSet Sts);              
    void Expression(StopSet Sts);                  
    void PrimaryOperator(StopSet Sts);             
    void PrimaryExpression(StopSet Sts);           
    void RelationalOperator(StopSet Sts);             
    void SimpleExpression(StopSet Sts);            
    void AddingOperator(StopSet Sts);                
    void Term(StopSet Sts);                        
    void MultiplyingOperator(StopSet Sts);         
    void Factor(StopSet Sts);                          
    void VariableAccess(StopSet Sts);              
    void IndexedSelector(StopSet Sts);             
    void Constant(StopSet Sts);                    
    void Numeral(StopSet Sts);                     
    void BooleanSymbol(StopSet Sts);                   
    void Name(StopSet Sts);                        
    /** End grammar rule definitions */

    Administration* Admin;
    Token* LookAheadToken;
};

#endif  //PARSER_H