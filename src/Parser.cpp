#include "../include/Parser.h"
#include <iostream>
#include <string>

#define PRINT(s) //cout << s << endl

//an array containing the follow set for each function, listed in the order the functions were defined in
//this does not need to be 50x50, but to avoid going over bounds, we pad the array a bit
Symbol::Symbol FirstSet[50][50] =
{
    {Symbol::BEGIN},
    {Symbol::BEGIN},
    {Symbol::CONST, Symbol::INTEGER, Symbol::BOOLEAN, Symbol::PROC},
    {Symbol::CONST, Symbol::INTEGER, Symbol::BOOLEAN, Symbol::PROC},
    {Symbol::CONST},
    {Symbol::INTEGER, Symbol::BOOLEAN},
    {Symbol::ID, Symbol::ARRAY},
    {Symbol::INTEGER, Symbol::BOOLEAN},
    {Symbol::ID},
    {Symbol::PROC},
    {Symbol::SKIP, Symbol::READ, Symbol::WRITE, Symbol::ID, Symbol::CALL, Symbol::IF, Symbol::DO},
    {Symbol::SKIP, Symbol::READ, Symbol::WRITE, Symbol::ID, Symbol::CALL, Symbol::IF, Symbol::DO},
    {Symbol::SKIP},
    {Symbol::READ},
    {Symbol::ID},
    {Symbol::WRITE},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::ID},
    {Symbol::CALL},
    {Symbol::IF},
    {Symbol::DO},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::AND, Symbol::OR},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::LESSTHAN, Symbol::EQUAL, Symbol::GREATERTHAN},
    {Symbol::MINUS, Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::PLUS, Symbol::MINUS},
    {Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::MULTIPLY, Symbol::DIVIDE, Symbol::MOD},
    {Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::BRACKETLEFT, Symbol::NEGATE},
    {Symbol::ID},
    {Symbol::SQUARELEFT},
    {Symbol::NUMERAL, Symbol::FALSE, Symbol::TRUE, Symbol::ID},
    {},
    {Symbol::FALSE, Symbol::TRUE},
    {},
};

Parser::Parser(Administration* A)
{
    Admin = A;

    GetNextToken();
}

void Parser::BeginParsing()
{
    StopSet Sts;
    Sts.insert(Symbol::ENDFILE);

    //call the first grammar rule, and let recursion handle the rest!
    Program(Sts);
}

void Parser::GetNextToken()
{
    //only get the next token if we are not at the end of the input file (no more tokens are coming)
    if(!LookAheadToken || !LookAheadToken->CheckTerminalSymbol(Symbol::ENDFILE))
    {
        LookAheadToken = Admin->GetNextToken();
    }
}

void Parser::Match(const Symbol::Symbol symbol, StopSet Sts)
{
    if (Check(symbol))
    {
        GetNextToken();
    }
    else
    {
        SyntaxError(Admin->TokenToString(symbol) + string(" before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
}

void Parser::SyntaxError(string Expected,  StopSet Sts)
{
    Admin->ReportError(string("Syntax Error --- Expected ") + Expected);

    while (!Member(LookAheadToken->GetSymbolName(), Sts))
    {
        GetNextToken();
    }
}

bool Parser::Member(const Symbol::Symbol symbol, StopSet Sts)
{
    return Sts.find(symbol) != Sts.end();
}

bool Parser::Member(const Symbol::Symbol symbol, int FirstSetID)
{
    int j = 0;
    while (FirstSet[FirstSetID][j] != Symbol::DUMMYVAL)
    {
        if (FirstSet[FirstSetID][j] == symbol)
        {
            return true;
        }
        ++j;
    }
    return false;
}

bool Parser::Check(const Symbol::Symbol symbol)
{
    return LookAheadToken->CheckTerminalSymbol(symbol);
}

StopSet Parser::CreateSet(int FirstSetIndex)
{
    StopSet S;

    int j = 0;
    while (FirstSet[FirstSetIndex][j] != Symbol::DUMMYVAL)
    {
        S.insert(FirstSet[FirstSetIndex][j]);
        ++j;
    }

    return S;
}

StopSet Parser::Union(const StopSet S1, const StopSet S2)
{
    StopSet Sts(S1);

    for (auto it = S2.begin(); it != S2.end(); ++it)
    {
        Sts.insert(*it);
    }

    return Sts;
}

StopSet Parser::Union(const StopSet S, const Symbol::Symbol symbol)
{
    StopSet Sts(S);

    Sts.insert(symbol);

    return Sts;
}

void Parser::Program(StopSet Sts)
{
    PRINT("Program");

    //Block(Union(Union(Union(Union(Sts, Symbol::PERIOD), CreateSet(EDefinitionPart)), CreateSet(EStatementPart)), Symbol::END));
    Block(Union(Sts, Symbol::PERIOD));

    Match(Symbol::PERIOD, Sts);
}

void Parser::Block(StopSet Sts)
{
    PRINT("Block");
    Match(Symbol::BEGIN, Union(Union(Union(Sts, CreateSet(EDefinitionPart)), CreateSet(EStatementPart)), Symbol::END));

    DefinitionPart(Union(Union(Union(Sts, CreateSet(EStatementPart)), Symbol::END), CreateSet(EDefinition)));

    StatementPart(Union(Union(Sts, Symbol::END), CreateSet(EStatement)));

    Match(Symbol::END, Sts);
}

void Parser::DefinitionPart(StopSet Sts)
{
    PRINT("DefinitionPart");
    while (Member(LookAheadToken->GetSymbolName(), EDefinition))
    {
        Definition(Union(Sts, Symbol::SEMICOLON));

        Match(Symbol::SEMICOLON, Sts);
    }
}

void Parser::Definition(StopSet Sts)
{
    if (Member(LookAheadToken->GetSymbolName(), EConstantDefinition))
    {
        ConstDefinition(Sts);
    }
    else if (Member(LookAheadToken->GetSymbolName(), EVariableDefinition))
    {
        VariableDefinition(Sts);
    }
    else if (Member(LookAheadToken->GetSymbolName(), EProcedureDefinition))
    {
        ProcedureDefinition(Sts);
    }
    else
    {
        SyntaxError(string("DEFINITION before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::ConstDefinition(StopSet Sts)
{
    PRINT("ConstDefinition");
    Match(Symbol::CONST, Union(Union(Union(Sts, Symbol::EQUAL), CreateSet(EName)), CreateSet(EConstant)));

    Name(Union(Union(Sts, Symbol::EQUAL), CreateSet(EConstant)));

    Match(Symbol::EQUAL, Union(Sts, CreateSet(EConstant)));

    Constant(Sts);
}

void Parser::VariableDefinition(StopSet Sts)
{
    PRINT("VariableDefinition");
    TypeSymbol(Union(Sts, CreateSet(EVariableDefinitionPrime)));

    VariableDefinitionPrime(Sts);
}

void Parser::VariableDefinitionPrime(StopSet Sts)
{
    PRINT("VariableDefinitionPrime");

    if (Check(Symbol::ARRAY))
    {
        Match(Symbol::ARRAY, Union(Union(Union(Union(Sts, CreateSet(EVariableList)), Symbol::SQUARELEFT), CreateSet(EConstant)), Symbol::SQUARERIGHT));

        VariableList(Union(Union(Union(Sts, Symbol::SQUARELEFT), CreateSet(EConstant)), Symbol::SQUARERIGHT));

        Match(Symbol::SQUARELEFT, Union(Union(Sts, CreateSet(EConstant)), Symbol::SQUARERIGHT));

        Constant(Union(Sts, Symbol::SQUARERIGHT));

        Match(Symbol::SQUARERIGHT, Sts);
    }
    else
    {
        VariableList(Sts);
    }
}

void Parser::TypeSymbol(StopSet Sts)
{
    PRINT("TypeSymbol");
    if (Check(Symbol::INTEGER))
    {
        Match(Symbol::INTEGER, Sts);
    }
    else if (Check(Symbol::BOOLEAN))
    {
        Match(Symbol::BOOLEAN, Sts);
    }
    else
    {
        SyntaxError(string("TYPE SYMBOL before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::VariableList(StopSet Sts)
{
    PRINT("VariableList");
    Name(Union(Sts, Symbol::COMMA)); //WARNING: is this correct?

    while (Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, Union(Sts, CreateSet(EName)));

        Name(Sts);
    }
}

void Parser::ProcedureDefinition(StopSet Sts)
{
    PRINT("ProcedureDefinition");
    Match(Symbol::PROC, Union(Union(Sts, CreateSet(EName)), CreateSet(EBlock)));

    Name(Union(Sts, CreateSet(EBlock)));

    Block(Sts);
}

void Parser::StatementPart(StopSet Sts)
{
    PRINT("StatementPart");
    while (Member(LookAheadToken->GetSymbolName(), EStatement))
    {
        Statement(Union(Sts, Symbol::SEMICOLON));

        Match(Symbol::SEMICOLON, Sts);
    }
}

void Parser::Statement(StopSet Sts)
{
    PRINT("Statement");
    if (Check(Symbol::SKIP))
    {
        EmptyStatement(Sts);
    }
    else if (Check(Symbol::READ))
    {
        ReadStatement(Sts);
    }
    else if (Check(Symbol::WRITE))
    {
        WriteStatement(Sts);
    }
    else if (Check(Symbol::ID))
    {
        AssignmentStatement(Sts);
    }
    else if (Check(Symbol::CALL))
    {
        ProcedureStatement(Sts);
    }
    else if (Check(Symbol::IF))
    {
        IfStatement(Sts);
    }
    else if (Check(Symbol::DO))
    {
        DoStatement(Sts);
    }
    else
    {
        SyntaxError(string("STATEMENT before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }    
}

void Parser::EmptyStatement(StopSet Sts)
{
    PRINT("EmptyStatement");
    Match(Symbol::SKIP, Sts);
}

void Parser::ReadStatement(StopSet Sts)
{
    PRINT("ReadStatement");
    Match(Symbol::READ, Union(Sts, CreateSet(EVariableAccessList)));

    VariableAccessList(Sts);
}

void Parser::VariableAccessList(StopSet Sts)
{
    PRINT("VariableAccessList");
    VariableAccess(Union(Sts, Symbol::COMMA)); //WARNING: is this correct?

    while (Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, Union(Sts, CreateSet(EVariableAccess)));

        VariableAccess(Sts);
    }
}

void Parser::WriteStatement(StopSet Sts)
{
    PRINT("WriteStatement");
    Match(Symbol::WRITE, Union(Sts, CreateSet(EExpressionList)));

    ExpressionList(Sts);
}

void Parser::ExpressionList(StopSet Sts)
{
    PRINT("ExpressionList");
    Expression(Union(Sts, Symbol::COMMA)); //WARNING: is this correct?

    while (Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, Union(Sts, CreateSet(EExpression)));

        Expression(Sts);
    }
}

void Parser::AssignmentStatement(StopSet Sts)
{
    PRINT("AssignmentStatement");
    VariableAccessList(Union(Union(Sts, Symbol::ASSIGN), CreateSet(EExpressionList)));

    Match(Symbol::ASSIGN, Union(Sts, CreateSet(EExpressionList)));

    ExpressionList(Sts);
}

void Parser::ProcedureStatement(StopSet Sts)
{
    PRINT("ProcedureStatement");
    Match(Symbol::CALL, Union(Sts, CreateSet(EName)));

    Name(Sts);
}

void Parser::IfStatement(StopSet Sts)
{
    PRINT("IfStatement");
    Match(Symbol::IF, Union(Union(Sts, CreateSet(EGuardedCommandList)), Symbol::FI));

    GuardedCommandList(Union(Sts, Symbol::FI));

    Match(Symbol::FI, Sts);
}

void Parser::DoStatement(StopSet Sts)
{
    PRINT("DoStatement");
    Match(Symbol::DO, Union(Union(Sts, CreateSet(EGuardedCommandList)), Symbol::OD));

    GuardedCommandList(Union(Sts, Symbol::OD));

    Match(Symbol::OD, Sts);
}

void Parser::GuardedCommandList(StopSet Sts)
{
    PRINT("GuardedCommandList");
    GuardedCommand(Union(Sts, Symbol::SUBSCRIPT)); //WARNING: is this correct?

    while (Check(Symbol::SUBSCRIPT))
    {
        Match(Symbol::SUBSCRIPT, Union(Sts, CreateSet(EGuardedCommand)));

        GuardedCommand(Sts);
    }
}

void Parser::GuardedCommand(StopSet Sts)
{
    PRINT("GuardedCommand");
    Expression(Union(Union(Sts, Symbol::ARROW), CreateSet(EStatementPart)));

    Match(Symbol::ARROW, Union(Sts, CreateSet(EStatementPart)));

    StatementPart(Sts);
}

void Parser::Expression(StopSet Sts)
{
    PRINT("Expression");
    PrimaryExpression(Union(Sts, CreateSet(EPrimaryOperator))); //WARNING: is this correct?

    while (Check(Symbol::AND) || Check(Symbol::OR))
    {
        PrimaryOperator(Union(Sts, CreateSet(EPrimaryExpression)));

        PrimaryExpression(Sts);
    }
}

void Parser::PrimaryOperator(StopSet Sts)
{
    PRINT("PrimaryOperator");
    if (Check(Symbol::AND))
    {
        Match(Symbol::AND, Sts);
    }
    else if(Check(Symbol::OR))
    {
        Match(Symbol::OR, Sts);
    }
    else
    {
        SyntaxError(string("PRIMARY OPERATOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::PrimaryExpression(StopSet Sts)
{
    PRINT("PrimaryExpression");
    SimpleExpression(Union(Sts, CreateSet(ERelationalOperator))); //WARNING: is this correct?

    if (Member(LookAheadToken->GetSymbolName(), ERelationalOperator))
    {
        RelationalOperator(Union(Sts, CreateSet(ESimpleExpression)));

        SimpleExpression(Sts);
    }
}

void Parser::RelationalOperator(StopSet Sts)
{
    PRINT("RelationalOperator");
    if (Check(Symbol::LESSTHAN))
    {
        Match(Symbol::LESSTHAN, Sts);
    }
    else if (Check(Symbol::EQUAL))
    {
        Match(Symbol::EQUAL, Sts);
    }
    else if (Check(Symbol::GREATERTHAN))
    {
        Match(Symbol::GREATERTHAN, Sts);
    }
    else
    {
        SyntaxError(string("RELATIONAL OPERATOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::SimpleExpression(StopSet Sts)
{
    PRINT("SimpleExpression");
    if (Check(Symbol::MINUS))
    {
        Match(Symbol::MINUS, Union(Sts, CreateSet(ETerm)));
    }

    Term(Union(Sts, CreateSet(EAddingOperator))); //WARNNING: is this correct?

    while (Member(LookAheadToken->GetSymbolName(), EAddingOperator))
    {
        AddingOperator(Union(Sts, CreateSet(ETerm)));

        Term(Sts);
    }
}

void Parser::AddingOperator(StopSet Sts)
{
    PRINT("AddingOperator");
    if (Check(Symbol::PLUS))
    {
        Match(Symbol::PLUS, Sts);
    }
    else if(Check(Symbol::MINUS))
    {
        Match(Symbol::MINUS, Sts);
    }
    else
    {
        SyntaxError(string("ADDING OPERATOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::Term(StopSet Sts)
{
    PRINT("Term");
    Factor(Union(Sts, CreateSet(EMultiplyingOperator))); //WARNING: is this correct?

    while (Member(LookAheadToken->GetSymbolName(), EMultiplyingOperator))
    {
        MultiplyingOperator(Union(Sts, CreateSet(EFactor)));

        Factor(Sts);
    }
}

void Parser::MultiplyingOperator(StopSet Sts)
{
    PRINT("MultiplyingOperator");
    if (Check(Symbol::MULTIPLY))
    {
        Match(Symbol::MULTIPLY, Sts);
    }
    else if (Check(Symbol::DIVIDE))
    {
        Match(Symbol::DIVIDE, Sts);
    }
    else if (Check(Symbol::MOD))
    {
        Match(Symbol::MOD, Sts);
    }
    else
    {
        SyntaxError(string("MULTIPLYING OPERATOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::Factor(StopSet Sts)
{
    PRINT("Factor");

    if (Check(Symbol::NUMERAL) || Check(Symbol::TRUE) || Check(Symbol::FALSE))
    {
        Constant(Sts);
    }
    else if (Check(Symbol::ID))
    {
        //this could also be a constant, however we will treat it as a variable access for now, and fix it in the later stage
        VariableAccess(Sts);
    }
    else if (Check(Symbol::BRACKETLEFT))
    {
        Match(Symbol::BRACKETLEFT, Union(Union(Sts, CreateSet(EExpression)), Symbol::BRACKETLEFT));
        Expression(Union(Sts, Symbol::BRACKETRIGHT));
        Match(Symbol::BRACKETRIGHT, Sts);
    }
    else if (Check(Symbol::NEGATE))
    {
        Match(Symbol::NEGATE, Union(Sts, CreateSet(EFactor)));
        Factor(Sts);
    }
    else
    {
        SyntaxError(string("FACTOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);    
    }
    
}

void Parser::VariableAccess(StopSet Sts)
{
    PRINT("VariableAccess");
    Name(Union(Sts, CreateSet(EIndexedSelector))); //WARNING: is this correct?

    if (Check(Symbol::SQUARELEFT))
    {
        IndexedSelector(Sts);
    }
}

void Parser::IndexedSelector(StopSet Sts)
{
    PRINT("IndexedSelector");
    Match(Symbol::SQUARELEFT, Union(Union(Sts, CreateSet(EExpression)), Symbol::SQUARERIGHT));

    Expression(Union(Sts, Symbol::SQUARERIGHT));

    Match(Symbol::SQUARERIGHT, Sts);
}

void Parser::Constant(StopSet Sts)
{
    PRINT("Constant");
    if (Check(Symbol::NUMERAL))
    {
        Numeral(Sts);
    }
    else if (Check(Symbol::TRUE) || Check(Symbol::FALSE))
    {
        BooleanSymbol(Sts);
    }
    else if (Check(Symbol::ID))
    {
        Name(Sts);
    }
    else
    {
        SyntaxError(string("CONSTANT before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::Numeral(StopSet Sts)
{
    PRINT("Numeral");
    Match(Symbol::NUMERAL, Sts);
}

void Parser::BooleanSymbol(StopSet Sts)
{
    PRINT("BooleanSymbol");
    if (Check(Symbol::TRUE))
    {
        Match(Symbol::TRUE, Sts);
    }
    else if (Check(Symbol::FALSE))
    {
        Match(Symbol::FALSE, Sts);
    }
    else
    {
        SyntaxError(string("BOOLEAN SYMBOL before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::Name(StopSet Sts)
{
    PRINT("Name");
    Match(Symbol::ID, Sts);
}