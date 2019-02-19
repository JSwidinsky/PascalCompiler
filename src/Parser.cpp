#include "../include/Parser.h"
#include <iostream>

#define PRINT_ERROR(n, s) cerr << "ERROR LINE " << n << ": " << s << endl;

Parser::Parser()
{
}

void Parser::BeginParsing()
{
    GetNextToken();

    //call the first grammar rule, and let recursion handle the rest!
    Program();
}

void Parser::GiveTokenQueue(queue<Token*>& TQ)
{
    TokenQueue = TQ;
}

void Parser::GetNextToken()
{
    LookAheadToken = TokenQueue.front();
    TokenQueue.pop();
}

void Parser::Program()
{
    Block();

    if(!LookAheadToken || !LookAheadToken->CheckTerminalSymbol(Symbol::PERIOD))
    {
        //bad!
        PRINT_ERROR(0, "Expected '.' at end of program declaration");
    }
}

void Parser::Block()
{
    if(!LookAheadToken->CheckTerminalSymbol(Symbol::BEGIN))
    {
        PRINT_ERROR(0, "Expected 'begin' at begining of block declaration");
    }
    else
    {
        GetNextToken();
    }

    DefinitionPart();

    if(!LookAheadToken->CheckTerminalSymbol(Symbol::END))
    {
        PRINT_ERROR(0, "Expected 'end' at end of block declaration");
    }

    GetNextToken();
}

void Parser::DefinitionPart()
{
    while(true)
    {
        if(LookAheadToken->CheckTerminalSymbol(Symbol::CONST))
        {
            //Const Definition!
            ConstDefinition();
        }
        else if(LookAheadToken->CheckTerminalSymbol(Symbol::INTEGER) || LookAheadToken->CheckTerminalSymbol(Symbol::BOOLEAN))
        {
            //Variable definition!
            VariableDefinition();
        }
        else if(LookAheadToken->CheckTerminalSymbol(Symbol::PROC))
        {
            //procedure definition!
            ProcedureDefinition();
        }
        else
        {
            break;
        }

        if(!LookAheadToken->CheckTerminalSymbol(Symbol::SEMICOLON))
        {
            PRINT_ERROR(0, "Expected ';' after definition statement");
        }
        else
        {
            GetNextToken();
        }
    }
}

void Parser::ConstDefinition()
{
    if(!LookAheadToken->CheckTerminalSymbol(Symbol::CONST))
    {
        PRINT_ERROR(0, "Expected 'const' before variable identifier declaration");
    }
    else
    {
        GetNextToken();
    }

    if(!LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        PRINT_ERROR(0, "Expected identifier after const declaration");
    }
    else
    {
        GetNextToken();
    }

    if(!LookAheadToken->CheckTerminalSymbol(Symbol::EQUAL))
    {
        PRINT_ERROR(0, "Expected '=' operator after const identifier declaration");
    }
    else
    {
        GetNextToken();
    }

    Constant();
}

void Parser::VariableDefinition()
{
    TypeSymbol();

    VariableDefinitionPrime();
}

void Parser::VariableDefinitionPrime()
{
    bool IsArray = false;
    if(LookAheadToken->CheckTerminalSymbol(Symbol::ARRAY))
    {
        GetNextToken();
        IsArray = true;
    }

    VariableList();

    if(IsArray)
    {
        if(!LookAheadToken->CheckTerminalSymbol(Symbol::SQUARELEFT))
        {
            PRINT_ERROR(0, "Expected '[' after array identifier declaration");
        }
        else
        {
            GetNextToken();
        }

        Constant();

        if(!LookAheadToken->CheckTerminalSymbol(Symbol::SQUARERIGHT))
        {
            PRINT_ERROR(0, "Expected ']' after array constant declaration");
        }
        else
        {
            GetNextToken();
        }
    }
}


void Parser::TypeSymbol()
{
    if(!LookAheadToken->CheckTerminalSymbol(Symbol::INTEGER) && !LookAheadToken->CheckTerminalSymbol(Symbol::BOOLEAN))
    {
        PRINT_ERROR(0, "Expected 'integer' or 'Boolean' at beginning of non-const variable definition");
    }
    else
    {
        GetNextToken();
    }
}

void Parser::VariableList()
{
    if(!LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        PRINT_ERROR(0, "Expected identifier after variable type declaration");
    }
    else
    {
        GetNextToken();
    }

    while(LookAheadToken->CheckTerminalSymbol(Symbol::COMMA))
    {
        GetNextToken();

        if(!LookAheadToken->CheckTerminalSymbol(Symbol::ID))
        {
            PRINT_ERROR(0, "Expected variable identifier declaration after comma in variable declaration list");
        }
        else
        {
            GetNextToken();
        }
    }
}

void Parser::ProcedureDefinition()
{
    if(!LookAheadToken->CheckTerminalSymbol(Symbol::PROC))
    {
        PRINT_ERROR(0, "Expected 'proc' at beginning of procedure definition");
    }
    else
    {
        GetNextToken();
    }

    if(!LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        PRINT_ERROR(0, " Expected procedure identifier after 'proc' declaration");
    }
    else
    {
        GetNextToken();
    }

    Block();
}


void Parser::Constant()
{
    if(LookAheadToken->CheckTerminalSymbol(Symbol::NUMERAL))
    {
        GetNextToken();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::BOOLEAN))
    {
        GetNextToken();
    }
    else if(LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        GetNextToken();
    }
    else
    {
        PRINT_ERROR(0, "Expected numeral, boolean, or const identifier as const declaration" );
    }
}