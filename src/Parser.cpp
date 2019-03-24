#include "../include/Parser.h"
#include <iostream>
#include <string>

using namespace std;

#define PRINT(s) //cout << s << endl
#define QUOTE_NAME(s) "'" + s + "' "

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
    
    Table = new BlockTable();

    GetNextToken();
}

Parser::~Parser()
{
    delete Table;
    LookAheadToken = nullptr;
    Admin = nullptr;
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

void Parser::ScopeError(string Message)
{
    Admin->ReportError("Scope error --- " + Message);
}

void Parser::TypeError(string Message)
{
    Admin->ReportError("Type error --- " + Message);
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
    if(!Table->NewBlock())
    {
        Admin->FatalError("Cannot create new block; block count exceeds maximum allowed blocks");
    }

    PRINT("Block");
    Match(Symbol::BEGIN, Union(Union(Union(Sts, CreateSet(EDefinitionPart)), CreateSet(EStatementPart)), Symbol::END));

    DefinitionPart(Union(Union(Union(Sts, CreateSet(EStatementPart)), Symbol::END), CreateSet(EDefinition)));

    StatementPart(Union(Union(Sts, Symbol::END), CreateSet(EStatement)));

    Match(Symbol::END, Sts);

    if(!Table->PopBlock())
    {
        Admin->FatalError("Cannot remove block; no block to remove");
    }
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
        //can we ever get here?
        SyntaxError(string("DEFINITION before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

void Parser::ConstDefinition(StopSet Sts)
{
    PRINT("ConstDefinition");
    Match(Symbol::CONST, Union(Union(Union(Sts, Symbol::EQUAL), CreateSet(EName)), CreateSet(EConstant)));

    string IDName;
    int HashIndex = Name(Union(Union(Sts, Symbol::EQUAL), CreateSet(EConstant)), IDName);

    Match(Symbol::EQUAL, Union(Sts, CreateSet(EConstant)));

    int Value;
    TableEntry::Type TypeOfConstant;
    Constant(Sts, Value, TypeOfConstant);

    if(!Table->Define(TableEntry(HashIndex, 1, TableEntry::CONSTANT, TypeOfConstant, Value, Table->GetCurrentLevel())))
    {
        ScopeError(QUOTE_NAME(IDName) + "was already declared in this scope");
    }
}

void Parser::VariableDefinition(StopSet Sts)
{
    PRINT("VariableDefinition");
    TableEntry::Type TokenType = TypeSymbol(Union(Sts, CreateSet(EVariableDefinitionPrime)));

    VariableDefinitionPrime(Sts, TokenType);
}

void Parser::VariableDefinitionPrime(StopSet Sts, TableEntry::Type VariableType)
{
    PRINT("VariableDefinitionPrime");

    if (Check(Symbol::ARRAY))
    {
        Match(Symbol::ARRAY, Union(Union(Union(Union(Sts, CreateSet(EVariableList)), Symbol::SQUARELEFT), CreateSet(EConstant)), Symbol::SQUARERIGHT));

        //this needs to return a vector of all the variables that it found and discovered
        vector<pair<int, string>> NameIndicies = VariableList(Union(Union(Union(Sts, Symbol::SQUARELEFT), CreateSet(EConstant)), Symbol::SQUARERIGHT));

        Match(Symbol::SQUARELEFT, Union(Union(Sts, CreateSet(EConstant)), Symbol::SQUARERIGHT));

        int ArraySize;
        TableEntry::Type ArraySizeType;
        Constant(Union(Sts, Symbol::SQUARERIGHT), ArraySize, ArraySizeType);

        //is this right? are all of the variables defined here arrays?
        if(ArraySize != -1)
        {
            for(const pair<int, string>& HashIndex : NameIndicies)
            {
                if(!Table->Define(TableEntry(HashIndex.first, ArraySize, TableEntry::ARRAY, VariableType, 0, Table->GetCurrentLevel())))
                {
                    ScopeError(QUOTE_NAME(HashIndex.second) + "was already declared in this scope");
                }
            }
        }

        Match(Symbol::SQUARERIGHT, Sts);
    }
    else
    {
        vector<pair<int, string>> NameIndicies = VariableList(Sts);

        for(const pair<int, string>& HashIndex : NameIndicies)
        {
            if(!Table->Define(TableEntry(HashIndex.first, 1, TableEntry::VARIABLE, VariableType, 0, Table->GetCurrentLevel())))
            {
                ScopeError(QUOTE_NAME(HashIndex.second) + "was already declared in this scope");
            }
        }
    }
}

TableEntry::Type Parser::TypeSymbol(StopSet Sts)
{
    PRINT("TypeSymbol");
    if (Check(Symbol::INTEGER))
    {
        Match(Symbol::INTEGER, Sts);
        return TableEntry::INTEGER;
    }
    else if (Check(Symbol::BOOLEAN))
    {
        Match(Symbol::BOOLEAN, Sts);
        return TableEntry::BOOLEAN;
    }
    else
    {
        //can we ever get here currently?
        SyntaxError(string("TYPE SYMBOL before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

vector<pair<int, string>> Parser::VariableList(StopSet Sts)
{
    PRINT("VariableList");
    
    string IDName;
    int HashTableIndex = Name(Union(Sts, Symbol::COMMA), IDName); //WARNING: is this correct?
    vector<pair<int, string>> HashIndexList;

    HashIndexList.push_back(pair<int, string>(HashTableIndex, IDName));
    while (Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, Union(Sts, CreateSet(EName)));

        HashTableIndex = Name(Sts, IDName);
        HashIndexList.push_back(pair<int, string>(HashTableIndex, IDName));
    }

    return HashIndexList;
}

void Parser::ProcedureDefinition(StopSet Sts)
{
    PRINT("ProcedureDefinition");
    Match(Symbol::PROC, Union(Union(Sts, CreateSet(EName)), CreateSet(EBlock)));

    string IDName;
    int ProcIndex = Name(Union(Sts, CreateSet(EBlock)), IDName);

    if(!Table->Define(TableEntry(ProcIndex, 0, TableEntry::PROCEDURE, TableEntry::UNIVERSAL, 0, Table->GetCurrentLevel())))
    {
        ScopeError(QUOTE_NAME(IDName) + "was already declared in this scope");
    }

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
        //can we ever get here?
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

vector<TableEntry::Type> Parser::VariableAccessList(StopSet Sts)
{
    PRINT("VariableAccessList");

    vector<TableEntry::Type> VariableTypes;
    pair<int, string> Info = VariableAccess(Union(Sts, Symbol::COMMA)); //WARNING: is this correct?

    bool WasSuccessful;
    TableEntry Entry = Table->Find(Info.first, WasSuccessful);

    if(WasSuccessful)
    {
        VariableTypes.push_back(Entry.MyType);
    }
    else
    {
        ScopeError(QUOTE_NAME(Info.second) + "is an undeclared identifier");
    }

    while (Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, Union(Sts, CreateSet(EVariableAccess)));

        Info = VariableAccess(Sts);

        Entry = Table->Find(Info.first, WasSuccessful);

        if(WasSuccessful)
        {
            VariableTypes.push_back(Entry.MyType);
        }
        else
        {
            ScopeError(QUOTE_NAME(Info.second) + "is an undeclared identifier");
        }
    }

    return VariableTypes;
}

void Parser::WriteStatement(StopSet Sts)
{
    PRINT("WriteStatement");
    Match(Symbol::WRITE, Union(Sts, CreateSet(EExpressionList)));

    ExpressionList(Sts);
}

vector<TableEntry::Type> Parser::ExpressionList(StopSet Sts)
{
    PRINT("ExpressionList");

    vector<TableEntry::Type> Types;
    
    TableEntry::Type ExpressionType = Expression(Union(Sts, Symbol::COMMA)); //WARNING: is this correct?
    Types.push_back(ExpressionType);

    while (Check(Symbol::COMMA))
    {
        Match(Symbol::COMMA, Union(Sts, CreateSet(EExpression)));

        ExpressionType = Expression(Sts);
        Types.push_back(ExpressionType);
    }

    return Types;
}

void Parser::AssignmentStatement(StopSet Sts)
{
    PRINT("AssignmentStatement");
    vector<TableEntry::Type> VariableTypes = VariableAccessList(Union(Union(Sts, Symbol::ASSIGN), CreateSet(EExpressionList)));

    Match(Symbol::ASSIGN, Union(Sts, CreateSet(EExpressionList)));

    vector<TableEntry::Type> ExpressionTypes = ExpressionList(Sts);

    if(VariableTypes.size() != ExpressionTypes.size())
    {
        TypeError("Mismatch between number of variables on left and right side of assignment operator");
    }
    else
    {
        for(unsigned int i = 0; i < VariableTypes.size(); ++i)
        {
            if(VariableTypes[i] != ExpressionTypes[i])
            {
                TypeError("Mismatch between variable type and expression type in assignment statement");
            }
        }
    }
}

void Parser::ProcedureStatement(StopSet Sts)
{
    PRINT("ProcedureStatement");
    Match(Symbol::CALL, Union(Sts, CreateSet(EName)));

    string IDName;
    int NameIndex = Name(Sts, IDName);

    bool WasSuccessful;
    TableEntry Entry = Table->Find(NameIndex, WasSuccessful);
    if(WasSuccessful)
    {
        if(Entry.MyKind != TableEntry::PROCEDURE)
        {
            TypeError(QUOTE_NAME(IDName) + "is not a procedure");
        }
    }
    else
    {
        ScopeError(QUOTE_NAME(IDName) + "was not declared in this scope");
    }
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
    TableEntry::Type ExpressionType = Expression(Union(Union(Sts, Symbol::ARROW), CreateSet(EStatementPart)));

    if(ExpressionType != TableEntry::BOOLEAN)
    {
        TypeError("Guarded command expression does not evaluate to boolean data type");
    }

    Match(Symbol::ARROW, Union(Sts, CreateSet(EStatementPart)));

    StatementPart(Sts);
}

TableEntry::Type Parser::Expression(StopSet Sts)
{
    PRINT("Expression");
    const TableEntry::Type ExpressionType = PrimaryExpression(Union(Sts, CreateSet(EPrimaryOperator))); //WARNING: is this correct?

    while (Check(Symbol::AND) || Check(Symbol::OR))
    {
        if(ExpressionType != TableEntry::BOOLEAN)
        {
            TypeError("Expression does not evaluate to a boolean value");
        }
        PrimaryOperator(Union(Sts, CreateSet(EPrimaryExpression)));

        TableEntry::Type ExpressionType2 = PrimaryExpression(Sts);

        if(ExpressionType2 != TableEntry::BOOLEAN)
        {
            TypeError("Expression does not evaluate to a boolean value");
        }
    }
    return ExpressionType;
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
        //can we ever get here currently?
        SyntaxError(string("PRIMARY OPERATOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
    
}

TableEntry::Type Parser::PrimaryExpression(StopSet Sts)
{
    PRINT("PrimaryExpression");
    TableEntry::Type ExpressionType = SimpleExpression(Union(Sts, CreateSet(ERelationalOperator))); //WARNING: is this correct?

    if (Member(LookAheadToken->GetSymbolName(), ERelationalOperator))
    {
        if(ExpressionType != TableEntry::INTEGER)
        {
            TypeError("Relational operator requires integer value type");
        }
        RelationalOperator(Union(Sts, CreateSet(ESimpleExpression)));

        TableEntry::Type ExpressionType2 = SimpleExpression(Sts);
        if(ExpressionType2 != TableEntry::INTEGER)
        {
            TypeError("Relational operator requres integer value type");
        }
        ExpressionType = TableEntry::BOOLEAN;
    }

    return ExpressionType;
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
        //can we ever get here currently?
        SyntaxError(string("RELATIONAL OPERATOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
}

TableEntry::Type Parser::SimpleExpression(StopSet Sts)
{
    PRINT("SimpleExpression");

    bool MatchedMinus = false;
    if (Check(Symbol::MINUS))
    {
        Match(Symbol::MINUS, Union(Sts, CreateSet(ETerm)));
    }

    const TableEntry::Type ExpressionType = Term(Union(Sts, CreateSet(EAddingOperator))); //WARNNING: is this correct?

    if(MatchedMinus && ExpressionType != TableEntry::INTEGER)
    {
        TypeError("Invalid use of negation on a boolean value");
    }

    while (Member(LookAheadToken->GetSymbolName(), EAddingOperator))
    {
        if(ExpressionType != TableEntry::INTEGER)
        {
            TypeError("Operator requires integer value type");
        }
        AddingOperator(Union(Sts, CreateSet(ETerm)));

        TableEntry::Type ExpressionType2 = Term(Sts);
        if(ExpressionType2 != TableEntry::INTEGER)
        {
            TypeError("Operator requires integer value type");
        }
    }
    return ExpressionType;
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

TableEntry::Type Parser::Term(StopSet Sts)
{
    PRINT("Term");
    const TableEntry::Type ExpressionType = Factor(Union(Sts, CreateSet(EMultiplyingOperator))); //WARNING: is this correct?

    while (Member(LookAheadToken->GetSymbolName(), EMultiplyingOperator))
    {
        if(ExpressionType != TableEntry::INTEGER)
        {
            TypeError("Operator requires integer value type");
        }
        MultiplyingOperator(Union(Sts, CreateSet(EFactor)));

        TableEntry::Type ExpressionType2 = Factor(Sts);
        if(ExpressionType2 != TableEntry::INTEGER)
        {
            TypeError("Operator requires integer value type");
        }
    }
    return ExpressionType;
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

TableEntry::Type Parser::Factor(StopSet Sts)
{
    PRINT("Factor");

    //here, we check to see if our look ahead token is an id, and if it is, we want to see if it is a constant, so that we may
    //call the appropriate grammar rule
    bool IsIDConstant = false;
    if(Check(Symbol::ID))
    {
        bool DummyBool;
        TableEntry Entry = Table->Find(LookAheadToken->GetValue(), DummyBool);
        if(DummyBool)
        {
            IsIDConstant = Entry.MyKind == TableEntry::CONSTANT;
        }
    }

    TableEntry::Type Type;
    if (Check(Symbol::NUMERAL) || Check(Symbol::TRUE) || Check(Symbol::FALSE) || IsIDConstant)
    {  
        int Value;
        Constant(Sts, Value, Type);
    }
    else if (Check(Symbol::ID))
    {
        pair<int, string> VariableInfo = VariableAccess(Sts);

        bool WasSuccessful;
        TableEntry Entry = Table->Find(VariableInfo.first, WasSuccessful);
        if(WasSuccessful)
        {
            Type = Entry.MyType;
        }
        else
        {
            ScopeError(QUOTE_NAME(VariableInfo.second) + "is an undeclared identifier");
        }
    }
    else if (Check(Symbol::BRACKETLEFT))
    {
        Match(Symbol::BRACKETLEFT, Union(Union(Sts, CreateSet(EExpression)), Symbol::BRACKETLEFT));
        Type = Expression(Union(Sts, Symbol::BRACKETRIGHT));
        Match(Symbol::BRACKETRIGHT, Sts);
    }
    else if (Check(Symbol::NEGATE))
    {
        Match(Symbol::NEGATE, Union(Sts, CreateSet(EFactor)));
        Type = Factor(Sts);

        if(Type != TableEntry::BOOLEAN)
        {
            TypeError("Negation requires boolean type on right-hand side of operator");
        }
    }
    else
    {
        SyntaxError(string("FACTOR before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);    
    }
    
    return Type;
}

pair<int, string> Parser::VariableAccess(StopSet Sts)
{
    PRINT("VariableAccess");
    
    string IDName;
    int Index = Name(Union(Sts, CreateSet(EIndexedSelector)), IDName); //WARNING: is this correct?

    if (Check(Symbol::SQUARELEFT))
    {
        TableEntry::Type ExpressionType = IndexedSelector(Sts);

        if(ExpressionType != TableEntry::INTEGER)
        {
            TypeError("Non-integer value used for array indexing");
        }

        //check to make sure the type is an array if we are here
        bool WasSuccessful;
        TableEntry Entry = Table->Find(Index, WasSuccessful);
        if(WasSuccessful)
        {
            if(Entry.MyKind != TableEntry::ARRAY)
            {
                TypeError(QUOTE_NAME(IDName) + "is not an array type");
            } 
        }
    }
    return pair<int, string>(Index, IDName);
}

TableEntry::Type Parser::IndexedSelector(StopSet Sts)
{
    PRINT("IndexedSelector");
    Match(Symbol::SQUARELEFT, Union(Union(Sts, CreateSet(EExpression)), Symbol::SQUARERIGHT));

    TableEntry::Type ExpressionType = Expression(Union(Sts, Symbol::SQUARERIGHT));

    Match(Symbol::SQUARERIGHT, Sts);

    return ExpressionType;
}

void Parser::Constant(StopSet Sts, int& Value, TableEntry::Type& TypeOfConstant)
{
    PRINT("Constant");
    if (Check(Symbol::NUMERAL))
    {
        Value = Numeral(Sts);
        TypeOfConstant = TableEntry::INTEGER;
    }
    else if (Check(Symbol::TRUE) || Check(Symbol::FALSE))
    {
        Value = BooleanSymbol(Sts);
        TypeOfConstant = TableEntry::BOOLEAN;
    }
    else if (Check(Symbol::ID))
    {
        string IDName;
        int HashIndex = Name(Sts, IDName);

        bool FindSuccessful;
        TableEntry Entry = Table->Find(HashIndex, FindSuccessful);

        if(FindSuccessful)
        {
            Value = Entry.Value;
            if(Entry.MyType == TableEntry::INTEGER)
            {
                TypeOfConstant = TableEntry::INTEGER;
            }
            else
            {
                TypeError("Invalid assignment of named boolean constant");
            }
        }
        else
        {
            TypeError(QUOTE_NAME(IDName) + "was not declared in this scope");
            Value = -1;
        }
    }
    else
    {
        SyntaxError(string("CONSTANT before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }
}

int Parser::Numeral(StopSet Sts)
{
    PRINT("Numeral");
    int Value = LookAheadToken->GetValue();
    Match(Symbol::NUMERAL, Sts);

    return Value;
}

int Parser::BooleanSymbol(StopSet Sts)
{
    PRINT("BooleanSymbol");
    if (Check(Symbol::TRUE))
    {
        Match(Symbol::TRUE, Sts);
        return 1;
    }
    else if (Check(Symbol::FALSE))
    {
        Match(Symbol::FALSE, Sts);
        return 0;
    }
    else
    {
        SyntaxError(string("BOOLEAN SYMBOL before ") + Admin->TokenToString(LookAheadToken->GetSymbolName()), Sts);
    }    
}

int Parser::Name(StopSet Sts, string& IDName)
{
    PRINT("Name");
    int HashIndex = LookAheadToken->GetValue();
    IDName = LookAheadToken->GetLexeme();
    Match(Symbol::ID, Sts);
    
    return HashIndex;
}