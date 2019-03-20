#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <fstream>
#include <string>
#include "Scanner.h"
#include "Parser.h"

//the maximum number of errors allowed before compilation bails
#define MAX_ERROR_COUNT 20

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

    void ReportError(std::string ErrMessage);

    string TokenToString(const Symbol::Symbol symbol);

private:
    ifstream& InFile;
    ofstream& OutFile;
    Scanner* PLScanner;
    Parser* PLParser;

    int ErrorCount;
};


#endif //ADMINISTRATION_H
