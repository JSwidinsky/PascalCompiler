#include "../include/Token.h"

#include <iostream>
using namespace std;

Token::Token()
    : Attribute(TokenAttribute(-1, ""))
{}

Token::Token(Symbol::Symbol Terminal, TokenAttribute Attr)
    : TerminalSymbolName(Terminal), Attribute(Attr) {}

string Token::GetLexeme() const
{
    return Attribute.Lexeme;
}

Symbol::Symbol Token::GetSymbolName() const
{
    return TerminalSymbolName;
}

int Token::GetValue() const
{
    return Attribute.Value;
}

bool Token::CheckTerminalSymbol(Symbol::Symbol SymbolToCompare) const
{
    return this->TerminalSymbolName == SymbolToCompare;
}

void Token::PrintToken() const
{
    cout << "Token Symbol: ";

    //this is a pretty horrid way to print the enum name and any extra data, but it works for now
    switch(this->TerminalSymbolName)
    {
        case Symbol::BEGIN: cout << "BEGIN" << endl; break;
        case Symbol::END: cout << "END" << endl; break;
        case Symbol::CONST: cout << "CONST" << endl; break;
        case Symbol::ARRAY: cout << "ARRAY" << endl; break;
        case Symbol::INTEGER: cout << "INTEGER" << endl; break;
        case Symbol::BOOLEAN: cout << "BOOLEAN" << endl; break;
        case Symbol::PROC: cout << "PROC" << endl; break;
        case Symbol::SKIP: cout << "SKIP" << endl; break;
        case Symbol::READ: cout << "READ" << endl; break;
        case Symbol::WRITE: cout << "WRITE" << endl; break;
        case Symbol::CALL: cout << "CALL" << endl; break;
        case Symbol::IF: cout << "IF" << endl; break;
        case Symbol::DO: cout << "DO" << endl; break;
        case Symbol::FI: cout << "FI" << endl; break;
        case Symbol::OD: cout << "OD" << endl; break;
        case Symbol::FALSE: cout << "FALSE" << endl; break;
        case Symbol::TRUE: cout << "TRUE" << endl; break;
        case Symbol::ID: cout << "ID " << this->GetLexeme() << endl; break;
        case Symbol::NUMERAL: cout << "NUMERAL " << this->GetValue() <<  endl; break;
        case Symbol::PERIOD: cout << "PERID" << endl; break;
        case Symbol::COMMA: cout << "COMMA" << endl; break;
        case Symbol::SEMICOLON: cout << "SEMICOLON" << endl; break;
        case Symbol::SQUARELEFT: cout << "SQUARELEFT" << endl; break;
        case Symbol::SQUARERIGHT: cout << "SQUARERIGHT" << endl; break;
        case Symbol::AND: cout << "AND" << endl; break;
        case Symbol::OR: cout << "OR" << endl; break;
        case Symbol::NEGATE: cout << "NEGATE" << endl; break;
        case Symbol::LESSTHAN: cout << "LESSTHAN" << endl; break;
        case Symbol::EQUAL: cout << "EQUAL" << endl; break;
        case Symbol::GREATERTHAN: cout << "GREATERTHAN" << endl; break;
        case Symbol::PLUS: cout << "PLUS" << endl; break;
        case Symbol::MINUS: cout << "MINUS" << endl; break;
        case Symbol::MULTIPLY: cout << "MULTIPLY" << endl; break;
        case Symbol::DIVIDE: cout << "DIVIDE" << endl; break;
        case Symbol::MOD: cout << "MOD" << endl; break;
        case Symbol::BRACKETLEFT: cout << "BRACKETLEFT" << endl; break;
        case Symbol::BRACKETRIGHT: cout << "BRACKETRIGHT" << endl; break;
        case Symbol::ASSIGN: cout << "ASSIGN" << endl; break;
        case Symbol::SUBSCRIPT: cout << "SUBSCRIPT" << endl; break;
        case Symbol::ARROW: cout << "ARROW" << endl; break;
    }


}