#include "../include/Parser.h"
#include <iostream>

#define PRINT(s) cout << s << endl

Parser::Parser(Administration* A)
{
    Admin = A;

    GetNextToken();
}

void Parser::BeginParsing()
{
    //call the first grammar rule, and let recursion handle the rest!
    Program();
}

void Parser::GetNextToken()
{
    LookAheadToken = Admin->GetNextToken();
}

void Parser::Match(const Symbol::Symbol symbol)
{
    if(LookAheadToken->CheckTerminalSymbol(symbol))
    {
        GetNextToken();
    }
    else
    {
        Admin->ReportError("");
    }
}

void Parser::Program()
{
    PRINT("Program");
    Block();

    Match(Symbol::PERIOD);
}

void Parser::Block()
{
    PRINT("Block");
    Match(Symbol::BEGIN);

    DefinitionPart();

    StatementPart();

    Match(Symbol::END);
}

void Parser::DefinitionPart()
{
    while(true)
    {
        PRINT("DefinitionPart");
        if(LookAheadToken->CheckTerminalSymbol(Symbol::CONST))
        {
            ConstDefinition();
        }
        else if(LookAheadToken->CheckTerminalSymbol(Symbol::INTEGER) || LookAheadToken->CheckTerminalSymbol(Symbol::BOOLEAN))
        {
            VariableDefinition();
        }
        else if(LookAheadToken->CheckTerminalSymbol(Symbol::PROC))
        {
            ProcedureDefinition();
        }
        else
        {
            break;
        }

        Match(Symbol::SEMICOLON);
    }
}

void Parser::ConstDefinition()
{
    PRINT("ConstDefinition");
    Match(Symbol::CONST);

    Name();

    Match(Symbol::EQUAL);

    Constant();
}

void Parser::VariableDefinition()
{
    PRINT("VariableDefinition");
    TypeSymbol();

    VariableDefinitionPrime();
}

void Parser::VariableDefinitionPrime()
{
    PRINT("VariableDefinitionPrime");
    bool IsArray = false;
    if(LookAheadToken->CheckTerminalSymbol(Symbol::ARRAY))
    {
        Match(Symbol::ARRAY);
        IsArray = true;
    }

    VariableList();

    if(IsArray)
    {
        Match(Symbol::SQUARELEFT);

        Constant();

        Match(Symbol::SQUARERIGHT);
    }
}


void Parser::TypeSymbol()
{
    PRINT("TypeSymbol");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::INTEGER))
    {
        Match(Symbol::INTEGER);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::BOOLEAN))
    {
        Match(Symbol::BOOLEAN);
    }
}

void Parser::VariableList()
{
    PRINT("VariableList");
    Name();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::COMMA))
    {
        Match(Symbol::COMMA);

        Name();
    }
}

void Parser::ProcedureDefinition()
{
    PRINT("ProcedureDefinition");
    Match(Symbol::PROC);

    Name();

    Block();
}

void Parser::StatementPart()
{
    while(true)
    {
        PRINT("StatementPart");
        if(LookAheadToken->CheckTerminalSymbol(Symbol::SKIP) || LookAheadToken->CheckTerminalSymbol(Symbol::READ)
            || LookAheadToken->CheckTerminalSymbol(Symbol::WRITE) || LookAheadToken->CheckTerminalSymbol(Symbol::ID)
            || LookAheadToken->CheckTerminalSymbol(Symbol::CALL) || LookAheadToken->CheckTerminalSymbol(Symbol::IF)
            || LookAheadToken->CheckTerminalSymbol(Symbol::DO))
        {
            Statement();
        }
        else
        {
            break;
        }

        Match(Symbol::SEMICOLON);
    }
}

void Parser::Statement()
{
    PRINT("Statement");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::SKIP))
    {
        EmptyStatement();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::READ))
    {
        ReadStatement();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::WRITE))
    {
        WriteStatement();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        AssignmentStatement();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::CALL))
    {
        ProcedureStatement();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::IF))
    {
        IfStatement();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::DO))
    {
        DoStatement();
    }
}

void Parser::EmptyStatement()
{
    PRINT("EmptyStatement");
    Match(Symbol::SKIP);
}

void Parser::ReadStatement()
{
    PRINT("ReadStatement");
    Match(Symbol::READ);

    VariableAccessList();
}

void Parser::VariableAccessList()
{
    PRINT("VariableAccessList");
    VariableAccess();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::COMMA))
    {
        Match(Symbol::COMMA);

        VariableAccess();
    }
}

void Parser::WriteStatement()
{
    PRINT("WriteStatement");
    Match(Symbol::WRITE);

    ExpressionList();
}

void Parser::ExpressionList()
{
    PRINT("ExpressionList");
    Expression();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::COMMA))
    {
        Match(Symbol::COMMA);

        Expression();
    }
}

void Parser::AssignmentStatement()
{
    PRINT("AssignmentStatement");
    VariableAccessList();

    Match(Symbol::ASSIGN);

    ExpressionList();
}

void Parser::ProcedureStatement()
{
    PRINT("ProcedureStatement");
    Match(Symbol::CALL);

    Name();
}

void Parser::IfStatement()
{
    PRINT("IfStatement");
    Match(Symbol::IF);

    GuardedCommandList();

    Match(Symbol::FI);
}

void Parser::DoStatement()
{
    PRINT("DoStatement");
    Match(Symbol::DO);

    GuardedCommandList();

    Match(Symbol::OD);
}

void Parser::GuardedCommandList()
{
    PRINT("GuardedCommandList");
    GuardedCommand();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::SUBSCRIPT))
    {
        Match(Symbol::SUBSCRIPT);

        GuardedCommand();
    }
}

void Parser::GuardedCommand()
{
    PRINT("GuardedCommand");
    Expression();

    Match(Symbol::ARROW);
    
    StatementPart();
}

void Parser::Expression()
{
    PRINT("Expression");
    PrimaryExpression();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::AND) || LookAheadToken->CheckTerminalSymbol(Symbol::OR))
    {
        PrimaryOperator();

        PrimaryExpression();
    }
}

void Parser::PrimaryExpression()
{
    PRINT("PrimaryExpression");
    SimpleExpression();

    if(LookAheadToken->CheckTerminalSymbol(Symbol::LESSTHAN) || LookAheadToken->CheckTerminalSymbol(Symbol::EQUAL)
       || LookAheadToken->CheckTerminalSymbol(Symbol::GREATERTHAN))
    {
        RelationalOperator();

        SimpleExpression();
    }
}

void Parser::RelationalOperator()
{
    PRINT("RelationalOperator");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::LESSTHAN))
    {
        Match(Symbol::LESSTHAN);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::EQUAL))
    {
        Match(Symbol::EQUAL);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::GREATERTHAN))
    {
        Match(Symbol::GREATERTHAN);
    }
}

void Parser::SimpleExpression()
{
    PRINT("SimpleExpression");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::MINUS))
    {
        Match(Symbol::MINUS);
    }

    Term();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::PLUS) || LookAheadToken->CheckTerminalSymbol(Symbol::MINUS))
    {
        AddingOperator();

        Term();
    }
}

void Parser::AddingOperator()
{
    PRINT("AddingOperator");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::PLUS))
    {
        Match(Symbol::PLUS);
    }
    else
    {
        Match(Symbol::MINUS);
    }
}

void Parser::Term()
{
    PRINT("Term");
    Factor();

    while(LookAheadToken->CheckTerminalSymbol(Symbol::MULTIPLY) || LookAheadToken->CheckTerminalSymbol(Symbol::DIVIDE)
        || LookAheadToken->CheckTerminalSymbol(Symbol::MOD))
    {
        MultiplyingOperator();

        Factor();
    }
}

void Parser::MultiplyingOperator()
{
    PRINT("MultiplyingOperator");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::MULTIPLY))
    {
        Match(Symbol::MULTIPLY);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::DIVIDE))
    {
        Match(Symbol::DIVIDE);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::MOD))
    {
        Match(Symbol::MOD);
    }
}

void Parser::Factor()
{
    PRINT("Factor");

    if(LookAheadToken->CheckTerminalSymbol(Symbol::NUMERAL) || LookAheadToken->CheckTerminalSymbol(Symbol::TRUE)
       || LookAheadToken->CheckTerminalSymbol(Symbol::FALSE))
    {
        Constant();    
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        VariableAccess();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::BRACKETLEFT))
    {
        Match(Symbol::BRACKETLEFT);
        Expression();
        Match(Symbol::BRACKETRIGHT);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::NEGATE))
    {
        Match(Symbol::NEGATE);
        Factor();
    }
}

void Parser::PrimaryOperator()
{
    PRINT("PrimaryOperator");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::AND))
    {
        Match(Symbol::AND);
    }
    else
    {
        Match(Symbol::OR);
    }
}

void Parser::VariableAccess()
{
    PRINT("VariableAccess");
    Name();

    if(LookAheadToken->CheckTerminalSymbol(Symbol::SQUARELEFT))
    {
        IndexedSelector();
    }
}

void Parser::IndexedSelector()
{
    PRINT("IndexedSelector");
    Match(Symbol::SQUARELEFT);

    Expression();

    Match(Symbol::SQUARERIGHT);   
}

void Parser::Constant()
{
    PRINT("Constant");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::NUMERAL))
    {
        Numeral();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::TRUE) || LookAheadToken->CheckTerminalSymbol(Symbol::FALSE))
    {
        BooleanSymbol();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        Name();
    }
}

void Parser::Numeral()
{
    PRINT("Numeral");
    Match(Symbol::NUMERAL);
}

void Parser::BooleanSymbol()
{
    PRINT("BooleanSymbol");
    if(LookAheadToken->CheckTerminalSymbol(Symbol::TRUE))
    {
        Match(Symbol::TRUE);
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::FALSE))
    {
        Match(Symbol::FALSE);
    }
}

void Parser::Name()
{
    PRINT("Name");
    Match(Symbol::ID);
}