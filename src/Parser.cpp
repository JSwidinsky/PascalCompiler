#include "../include/Parser.h"
#include <iostream>
#include <string>

#define PRINT(s) cout << s << endl

//an array containing the follow set for each function, listed in the order the functions were defined in
Symbol::Symbol FollowSet[38][25] = 
{
    {},
    {Symbol::PERIOD, Symbol::SEMICOLON},
    {Symbol::SKIP, Symbol::READ, Symbol::WRITE, Symbol::ID, Symbol::CALL, Symbol::IF, Symbol::DO, Symbol::END},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::ID, Symbol::ARRAY},
    {Symbol::SEMICOLON, Symbol::SQUARELEFT},
    {Symbol::SEMICOLON},
    {Symbol::END, Symbol::SUBSCRIPT, Symbol::FI, Symbol::OD},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::SEMICOLON},
    {Symbol::FI, Symbol::OD},
    {Symbol::SUBSCRIPT, Symbol::FI, Symbol::OD},
    {Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETRIGHT, Symbol::NEGATE},
    {Symbol::COMMA, Symbol::SEMICOLON,Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::ID, Symbol::BEGIN, Symbol::SQUARELEFT, Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD, Symbol::PLUS, Symbol::MINUS, Symbol::COMMA, Symbol::SEMICOLON, Symbol::ARROW, Symbol::BRACKETRIGHT, Symbol::SQUARERIGHT, Symbol::AND, Symbol::OR, Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
};

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

void Parser::Match(const Symbol::Symbol symbol, int FunctionID)
{
    if(Check(symbol))
    {
        GetNextToken();
    }
    else
    {
        Admin->ReportError(string("Syntax Error --- Expected ") + Admin->TokenToString(symbol) + string(" before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()));
        while(!Member(LookAheadToken->GetSymbolName(), FunctionID))
        {
            GetNextToken();
        }
    }
}

bool Parser::Member(const Symbol::Symbol symbol, int FunctionID)
{
    for(int i = 0; i < 25; i++)
    {
        if(FollowSet[FunctionID][i] == symbol)
        {
            return true;
        }
    }
    return false;
}

bool Parser::Check(const Symbol::Symbol symbol)
{
    return LookAheadToken->CheckTerminalSymbol(symbol);
}

void Parser::Program()
{
    PRINT("Program");
    Block();

    Match(Symbol::PERIOD, 0);
}

void Parser::Block()
{
    PRINT("Block");
    Match(Symbol::BEGIN, 1);

    DefinitionPart();

    StatementPart();

    Match(Symbol::END, 1);
}

void Parser::DefinitionPart()
{
    PRINT("DefinitionPart");
    if(Check(Symbol::CONST) || Check(Symbol::INTEGER) || Check(Symbol::BOOLEAN) || Check(Symbol::PROC))
    {
        Definition();
    }
    else
    {
        return;
    }

    Match(Symbol::SEMICOLON, 2);

    DefinitionPart();
}

void Parser::Definition()
{
    if(Check(Symbol::CONST))
    {
        ConstDefinition();
    }
    else if(Check(Symbol::INTEGER) || Check(Symbol::BOOLEAN))
    {
        VariableDefinition();
    }
    else if(Check(Symbol::PROC))
    {
        ProcedureDefinition();
    }
}

void Parser::ConstDefinition()
{
    PRINT("ConstDefinition");
    Match(Symbol::CONST, 3);

    Name();

    Match(Symbol::EQUAL, 4);

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
    if(Check(Symbol::ARRAY))
    {
        Match(Symbol::ARRAY, 6);
        IsArray = true;
    }

    VariableList();

    if(IsArray)
    {
        Match(Symbol::SQUARELEFT, 6);

        Constant();

        Match(Symbol::SQUARERIGHT, 6);
    }
}


void Parser::TypeSymbol()
{
    PRINT("TypeSymbol");
    if(Check(Symbol::INTEGER))
    {
        Match(Symbol::INTEGER, 7);
    }
    else if(Check(Symbol::BOOLEAN))
    {
        Match(Symbol::BOOLEAN, 7);
    }
}

void Parser::VariableList()
{
    PRINT("VariableList");
    Name();

    while(Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, 8);

        Name();
    }
}

void Parser::ProcedureDefinition()
{
    PRINT("ProcedureDefinition");
    Match(Symbol::PROC, 9);

    Name();

    Block();
}

void Parser::StatementPart()
{
    PRINT("StatementPart");
    if(Check(Symbol::SKIP) || Check(Symbol::READ) || Check(Symbol::WRITE) || Check(Symbol::ID) || Check(Symbol::CALL) 
        || Check(Symbol::IF) || Check(Symbol::DO))
    {
        Statement();
    }
    else
    {
        return;
    }

    Match(Symbol::SEMICOLON, 10);

    StatementPart();
}

void Parser::Statement()
{
    PRINT("Statement");
    if(Check(Symbol::SKIP))
    {
        EmptyStatement();
    }
    else if(Check(Symbol::READ))
    {
        ReadStatement();
    }
    else if(Check(Symbol::WRITE))
    {
        WriteStatement();
    }
    else if(Check(Symbol::ID))
    {
        AssignmentStatement();
    }
    else if(Check(Symbol::CALL))
    {
        ProcedureStatement();
    }
    else if(Check(Symbol::IF))
    {
        IfStatement();
    }
    else if(Check(Symbol::DO))
    {
        DoStatement();
    }
}

void Parser::EmptyStatement()
{
    PRINT("EmptyStatement");
    Match(Symbol::SKIP, 12);
}

void Parser::ReadStatement()
{
    PRINT("ReadStatement");
    Match(Symbol::READ, 13);

    VariableAccessList();
}

void Parser::VariableAccessList()
{
    PRINT("VariableAccessList");
    VariableAccess();

    while(Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, 14);

        VariableAccess();
    }
}

void Parser::WriteStatement()
{
    PRINT("WriteStatement");
    Match(Symbol::WRITE, 15);

    ExpressionList();
}

void Parser::ExpressionList()
{
    PRINT("ExpressionList");
    Expression();

    while(Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, 16);

        Expression();
    }
}

void Parser::AssignmentStatement()
{
    PRINT("AssignmentStatement");
    VariableAccessList();

    Match(Symbol::ASSIGN, 17);

    ExpressionList();
}

void Parser::ProcedureStatement()
{
    PRINT("ProcedureStatement");
    Match(Symbol::CALL, 18);

    Name();
}

void Parser::IfStatement()
{
    PRINT("IfStatement");
    Match(Symbol::IF, 19);

    GuardedCommandList();

    Match(Symbol::FI, 19);
}

void Parser::DoStatement()
{
    PRINT("DoStatement");
    Match(Symbol::DO, 20);

    GuardedCommandList();

    Match(Symbol::OD, 20);
}

void Parser::GuardedCommandList()
{
    PRINT("GuardedCommandList");
    GuardedCommand();

    while(Check(Symbol::SUBSCRIPT))
    {
        Match(Symbol::SUBSCRIPT, 21);

        GuardedCommand();
    }
}

void Parser::GuardedCommand()
{
    PRINT("GuardedCommand");
    Expression();

    Match(Symbol::ARROW, 22);
    
    StatementPart();
}

void Parser::Expression()
{
    PRINT("Expression");
    PrimaryExpression();

    while(Check(Symbol::AND) || Check(Symbol::OR))
    {
        PrimaryOperator();

        PrimaryExpression();
    }
}

void Parser::PrimaryOperator()
{
    PRINT("PrimaryOperator");
    if(Check(Symbol::AND))
    {
        Match(Symbol::AND, 24);
    }
    else
    {
        Match(Symbol::OR, 24);
    }
}

void Parser::PrimaryExpression()
{
    PRINT("PrimaryExpression");
    SimpleExpression();

    if(Check(Symbol::LESSTHAN) || Check(Symbol::EQUAL) || Check(Symbol::GREATERTHAN))
    {
        RelationalOperator();

        SimpleExpression();
    }
}

void Parser::RelationalOperator()
{
    PRINT("RelationalOperator");
    if(Check(Symbol::LESSTHAN))
    {
        Match(Symbol::LESSTHAN, 26);
    }
    else if(Check(Symbol::EQUAL))
    {
        Match(Symbol::EQUAL, 26);
    }
    else if(Check(Symbol::GREATERTHAN))
    {
        Match(Symbol::GREATERTHAN, 26);
    }
}

void Parser::SimpleExpression()
{
    PRINT("SimpleExpression");
    if(Check(Symbol::MINUS))
    {
        Match(Symbol::MINUS, 27);
    }

    Term();

    while(Check(Symbol::PLUS) || Check(Symbol::MINUS))
    {
        AddingOperator();

        Term();
    }
}

void Parser::AddingOperator()
{
    PRINT("AddingOperator");
    if(Check(Symbol::PLUS))
    {
        Match(Symbol::PLUS, 28);
    }
    else
    {
        Match(Symbol::MINUS, 28);
    }
}

void Parser::Term()
{
    PRINT("Term");
    Factor();

    while(Check(Symbol::MULTIPLY) || Check(Symbol::DIVIDE) || Check(Symbol::MOD))
    {
        MultiplyingOperator();

        Factor();
    }
}

void Parser::MultiplyingOperator()
{
    PRINT("MultiplyingOperator");
    if(Check(Symbol::MULTIPLY))
    {
        Match(Symbol::MULTIPLY, 30);
    }
    else if(Check(Symbol::DIVIDE))
    {
        Match(Symbol::DIVIDE, 30);
    }
    else if(Check(Symbol::MOD))
    {
        Match(Symbol::MOD, 30);
    }
}

void Parser::Factor()
{
    PRINT("Factor");

    if(Check(Symbol::NUMERAL) || Check(Symbol::TRUE)
       || Check(Symbol::FALSE))
    {
        Constant();    
    }
    else if(Check(Symbol::ID))
    {
        //this could also be a constant, however we will treat it as a variable access for now, and fix it in the later stage
        VariableAccess();
    }
    else if(Check(Symbol::BRACKETLEFT))
    {
        Match(Symbol::BRACKETLEFT, 31);
        Expression();
        Match(Symbol::BRACKETRIGHT, 31);
    }
    else if(Check(Symbol::NEGATE))
    {
        Match(Symbol::NEGATE, 31);
        Factor();
    }
}

void Parser::VariableAccess()
{
    PRINT("VariableAccess");
    Name();

    if(Check(Symbol::SQUARELEFT))
    {
        IndexedSelector();
    }
}

void Parser::IndexedSelector()
{
    PRINT("IndexedSelector");
    Match(Symbol::SQUARELEFT, 33);

    Expression();

    Match(Symbol::SQUARERIGHT, 33);
}

void Parser::Constant()
{
    PRINT("Constant");
    if(Check(Symbol::NUMERAL))
    {
        Numeral();
    }
    else if(Check(Symbol::TRUE) || Check(Symbol::FALSE))
    {
        BooleanSymbol();
    }
    else if(Check(Symbol::ID))
    {
        Name();
    }
}

void Parser::Numeral()
{
    PRINT("Numeral");
    Match(Symbol::NUMERAL, 35);
}

void Parser::BooleanSymbol()
{
    PRINT("BooleanSymbol");
    if(Check(Symbol::TRUE))
    {
        Match(Symbol::TRUE, 36);
    }
    else if(Check(Symbol::FALSE))
    {
        Match(Symbol::FALSE, 36);
    }
}

void Parser::Name()
{
    PRINT("Name");
    Match(Symbol::ID, 37);
}