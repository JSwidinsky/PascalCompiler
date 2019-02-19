#include "../include/Parser.h"
#include <iostream>

#define ERROR_TEXT "ERROR LINE _____:"

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
        cerr << ERROR_TEXT <<  " Expected '.' at end of program declaration" << endl;
    }
}

void Parser::Block()
{
    if(!LookAheadToken->CheckTerminalSymbol(Symbol::BEGIN))
    {
        cerr << ERROR_TEXT << " Expected 'begin' at begining of block declaration" << endl;
    }
    else
    {
        GetNextToken();
    }

    DefinitionPart();

    if(!LookAheadToken->CheckTerminalSymbol(Symbol::END))
    {
        cerr << ERROR_TEXT << " Expected 'end' at end of block declaration" << endl;
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
            cerr << ERROR_TEXT << " Expected ';' after definition statement" << endl;
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
        cerr << ERROR_TEXT << " Expected const declaration" << endl;
    }

    GetNextToken();

    if(!(LookAheadToken->GetSymbolName() == Symbol::ID))
    {
        cerr << ERROR_TEXT << " Expected identifier after const declaration" << endl;
    }

    GetNextToken();

    if(!(LookAheadToken->GetSymbolName() == Symbol::EQUAL))
    {
        cerr << ERROR_TEXT << " Expected '=' operator after const identifier declaration" << endl;
    }

    GetNextToken();

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
            cerr << ERROR_TEXT << " Expected '[' after array identifier declaration" << endl;
        }
        else
        {
            GetNextToken();
        }

        Constant();

        if(!LookAheadToken->CheckTerminalSymbol(Symbol::SQUARERIGHT))
        {
            cerr << ERROR_TEXT << " Expected ']' after array constant declaration" << endl;
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
        cerr << ERROR_TEXT << " Expected 'integer' or 'Boolean' at beginning of non-const variable definition" << endl;
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
        cerr << ERROR_TEXT << " Expected identifier after variable type declaration" << endl;
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
            cerr << ERROR_TEXT << " Expected variable identifier declaration after comma in variable declaration list" << endl;
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
        cerr << ERROR_TEXT << " Expected 'proc' at beginning of procedure definition" << endl;
    }
    else
    {
        GetNextToken();
    }

    if(!LookAheadToken->CheckTerminalSymbol(Symbol::ID))
    {
        cerr << ERROR_TEXT << " Expected procedure identifier after 'proc' declaration" << endl;
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
        cerr << ERROR_TEXT << " Expected numeral, boolean, or const identifier as const declaration" << endl;
    }
}