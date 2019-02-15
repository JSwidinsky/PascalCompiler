#include "../include/Parser.h"
#include <iostream>

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

    if(LookAheadToken->GetSymbolName() == Symbol::PERIOD)
    {
        //good!
    }
    else
    {
        //bad!
    }
    
}

void Parser::Block()
{
    if(LookAheadToken->GetSymbolName() == Symbol::BEGIN)
    {
        //good!
        GetNextToken();
    }
    else
    {
        cerr << "ERROR: Expected begin" << endl;
    }

    DefinitionPart();

    if(LookAheadToken->GetSymbolName() == Symbol::END)
    {
        //good!
        GetNextToken();
    }
}

void Parser::DefinitionPart()
{
    while(true)
    {
        if(LookAheadToken->GetSymbolName() == Symbol::CONST)
        {
            //Const Definition!
            ConstDefinition();

            if(LookAheadToken->GetSymbolName() == Symbol::SEMICOLON)
            {
                //good!
                GetNextToken();
            }
        }
        else if(LookAheadToken->GetSymbolName() == Symbol::INTEGER || LookAheadToken->GetSymbolName() == Symbol::BOOLEAN)
        {
            //Variable definition!
            VariableDefinition();

            if(LookAheadToken->GetSymbolName() == Symbol::SEMICOLON)
            {
                //good!
                GetNextToken();
            }
        }
        else if(LookAheadToken->GetSymbolName() == Symbol::PROC)
        {
            //procedure definition!
            ProcedureDefinition();

            if(LookAheadToken->GetSymbolName() == Symbol::SEMICOLON)
            {
                //good!
                GetNextToken();
            }
        }
        else
        {
            break;
        }
    }
}

void Parser::ConstDefinition()
{
    if(LookAheadToken->GetSymbolName() == Symbol::CONST)
    {
        //good!
        GetNextToken();
    }
    else
    {
        cerr << "ERROR: Expected const declaration" << endl;
    }
    

    if(LookAheadToken->GetSymbolName() == Symbol::ID)
    {
        //good!
        GetNextToken();
    }
    else
    {
        cerr << "ERROR: Expected identifier" << endl;
    }
    

    if(LookAheadToken->GetSymbolName() == Symbol::EQUAL)
    {
        //good!
        GetNextToken();
    }
    else
    {
        cerr << "ERROR: Expected '=' operator after const identifier" << endl;
    }
    

    //call the constant function!
}

void Parser::VariableDefinition()
{

}

void Parser::ProcedureDefinition()
{

}


