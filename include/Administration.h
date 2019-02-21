#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <fstream>
#include <string>
#include "Scanner.h"
#include "./Parser.h"

//the maximum number of errors allowed before compilation bails
#define MAX_ERROR_COUNT 10

class Parser;

class Administration
{
public:
    Administration(ifstream& InputFile, ofstream& OutputFile);

    void Compile();

    /** Call the scanner to scan the input file and return the next token it finds */
    Token* GetNextToken();
    
    /** Call the parser to parse the input tokens */
    void Parse();

    void ReportError(std::string ErrMessage);

private:
    void PrintToken(Token* TokenToPrint) const;

    ifstream& InFile;
    ofstream& OutFile;
    Scanner* PLScanner;
    Parser* PLParser;

    int ErrorCount;
};


#endif //ADMINISTRATION_H
