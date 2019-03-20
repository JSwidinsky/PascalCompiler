#ifndef PARSER_H
#define PARSER_H

#include <set>
#include "./Administration.h"
#include "./Token.h"
#include "./BlockTable.h"

//forward declaration of admin class
class Administration;

typedef set<Symbol::Symbol> StopSet;

/** Used to encode values to each grammar rule function; begins with an E to differentiate it from the function itself */
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

    ~Parser();

    /**
     * Begins parsing the tokens
     */
    void BeginParsing();

private:
    /**
     * Gets next token from the scanner (via the Admin data member)
     */
    void GetNextToken();

    /**
     * Matches a given symbol with the look-ahead token; if they agree, the next token is fetched
     * If they do not agree, the function calls a syntax error
     * @param symbol Token symbol we are to compare with the look ahead token
     */
    void Match(const Symbol::Symbol symbol, StopSet Sts);

    /**
     * Report a syntax error to the administration class
     * @param Expected String detailing what was expected before an error was incurred
     * @param Sts StopSet of all terminals that can appear next
     */
    void SyntaxError(string Expected, StopSet Sts);

    /**
     * Checks if a symbol is in the FOLLOW set for a particular non-terminal, which is passed as the function id
     * @param symbol terminal symbol we are checking for membership in the follow set
     * @param FunctionID id of the function that the parser is currently in
     * @return true if symbol is in the FOLLOW set, false otherwise
     */
    bool Member(const Symbol::Symbol symbol, StopSet Sts);

    /**
     * Overloaded Member function that checks if a symbol is in the FIRST set of a given function
     * @param symbol Terminal symbol we want to check for membership
     * @param FirstSetID The numerical ID given to the function who we want to check for membership in
     */
    bool Member(const Symbol::Symbol symbol, int FirstSetID);

    /**
     * Helper function that checks if symbol matches with the symbol of the look ahead token
     */
    bool Check(const Symbol::Symbol symbol);

    /**
     * Creates a new StopSet given the FirstSetID (we create a StopSet for the FIRST set defined in the .cpp file)
     * @param FirstSetIndex The index of the set we want to create a stop set for
     */
    StopSet CreateSet(int FirstSetIndex);

    /**
     * Combines/unions two stop sets and returns this new set
     * @param S1 First stop set
     * @param S2 Second stop set
     */
    StopSet Union(const StopSet S1, const StopSet S2);

    /**
     * Combines/unions a stop set with a single terminal symbol and returns this new set
     * @param S Stop set we are adding terminal symbol to
     * @param symbol Symbol we want to add to stop set
     */
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
    void VariableDefinitionPrime(StopSet Sts, TableEntry::Type VariableType);     
    TableEntry::Type TypeSymbol(StopSet Sts);   //returns the type of the token that was matched              
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
    void Constant(StopSet Sts, int& Value, TableEntry::Type& TypeOfConstant);                    
    int Numeral(StopSet Sts);       //numeral returns the value of the literal number       
    int BooleanSymbol(StopSet Sts); //boolean symbol returns 0 if the bool was false, 1 if the bool was true
    int Name(StopSet Sts);                
    /** End grammar rule definitions */

    Administration* Admin;
    Token* LookAheadToken;
    BlockTable* Table;
};

#endif  //PARSER_H