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

void Token::UpdtateValue(const int NewValue)
{
    Attribute.Value = NewValue;
}

bool Token::CheckTerminalSymbol(Symbol::Symbol SymbolToCompare) const
{
    return this->TerminalSymbolName == SymbolToCompare;
}