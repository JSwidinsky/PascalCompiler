#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <string>
#include "Token.h"
#include "SymbolTable.h"

#define MAX_INT 4294967295

class Scanner
{
public:
    Scanner(ifstream& InputPLFile, SymbolTable* Table);

    /**
     * Function that reads the next part of the input file and creates a token for whatever it reads
     * This token is also added to the hash table before the function returns
     * @return The newly created token corresponding to the next section of the input file
     */
    Token* GetToken();

    /** Simple function that determines if we have made it to the end of the input file */
    bool AtEndOfFile() const;


private:
    /** Helper function that returns true if the character c is an alphabetic character */
    bool IsAlpha(const char c) const;

    /** Helper function that returns true if the character c is a numeric character */
    bool IsNumeric(const char c) const;

    /** Helper function that returns true if the character c is a blank/whitespace character */
    bool IsBlank(const char c) const;

    /** Helper function that returns true if the character c is a special character (operator, period, etc.) */
    bool IsSpecial(const char c) const;

    /** Helper function to recognize reserved words and IDs. Also adds the new token to the hash table, if required */
    Token* RecognizeName();

    /** Helper function to recognize a numeral in the input */
    Token* RecognizeNumeral();

    /** Helper function to recognize special symbols in the input */
    Token* RecognizeSpecial();

    /** Remove comments and white space from the file until we reach a valid character */
    void RecognizeSeparators();

    void GetNextSymbol();

    /** Helper function to place the lookahead char back in the input file */
    void ReverseRead();

    /** Helper function to remove an entire line of input */
    void RemoveLine();


    ifstream& InputPLProgram;
    SymbolTable* HashTable;
    char CurrentSymbol;
    char LookAheadSymbol;
    int LineNum;
};


#endif //SCANNER_H
