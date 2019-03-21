#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "Symbol.h"

using namespace std;

struct TokenAttribute
{
    TokenAttribute(int V, string Lex)
    : Value(V), Lexeme(Lex) {}
    /**
     * Value related to this token
     * For numerical literals, this is the value of that number
     * For reserved words and identifiers, this is the location of the token in the hashtable
     */
    int Value;

    /** The lexeme related to this token */
    string Lexeme;
};

/**
 * Interface for a token created during the scanning phase of the compiler
 *
 * A token contains both a lexeme and various attributes associated with it, such as symbol type
 */
class Token
{
public:
    //Constructors
    Token();
    Token(Symbol::Symbol Terminal, TokenAttribute Attr);

    //getter functions
    string GetLexeme() const;
    Symbol::Symbol GetSymbolName() const;
    int GetValue() const;
    void UpdtateValue(const int NewValue);

    bool CheckTerminalSymbol(Symbol::Symbol SymbolToCompare) const;

private:
    Symbol::Symbol TerminalSymbolName;
    TokenAttribute Attribute;

    /** The line number where this token was found and created */
    int LineNum;
};


#endif //TOKEN_H
