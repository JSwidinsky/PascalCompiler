#ifndef SYMBOL_H
#define SYMBOL_H

/**
 *
 */
 //wrap this in a namespace so that we can use :: when accessing the enum (used for clarity)
namespace Symbol
{
    enum Symbol
    {
        DUMMYVAL,
        //Keywords of the language PL
        BEGIN, END, CONST, ARRAY, INTEGER, BOOLEAN, PROC, SKIP,
        READ, WRITE, CALL, IF, DO, FI, OD, FALSE, TRUE,

        //
        ID, NUMERAL,

        //Special symbols in the language of PL
        PERIOD, COMMA, SEMICOLON, SQUARELEFT, SQUARERIGHT, AND, OR, NEGATE, LESSTHAN,
        EQUAL, GREATERTHAN, PLUS, MINUS, MULTIPLY, DIVIDE, MOD, BRACKETLEFT, BRACKETRIGHT,
        ASSIGN, SUBSCRIPT, ARROW
    };
}


#endif //SYMBOL_H
