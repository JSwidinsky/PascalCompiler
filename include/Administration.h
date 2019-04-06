#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <fstream>
#include <string>
#include "Scanner.h"
#include "Parser.h"

//the maximum number of errors allowed before compilation bails
#define MAX_ERROR_COUNT 30

//forward declaration of parser class
class Parser;

class Administration
{
public:
    Administration(ifstream& InputFile, ofstream& OutputFile);

    ~Administration();

    void Compile();

    /** Call the scanner to scan the input file and return the next token it finds */
    Token* GetNextToken();
    Token* GetTokenFromHashTable(const int Index) const;

    void Emit1(std::string Opcode);
    void Emit2(std::string Opcode, int Value1);
    void Emit3(std::string Opcode, int Value1, int Value2);

    void ReportError(std::string ErrMessage);
    void FatalError(std::string ErrMessage);

    string TokenToString(const Symbol::Symbol symbol);

    bool IsEmitting() const;
private:

    ifstream& InFile;
    ofstream& OutFile;
    Scanner* PLScanner;
    Parser* PLParser;
    SymbolTable* HashTable;

    int ErrorCount;
};


#endif //ADMINISTRATION_H
