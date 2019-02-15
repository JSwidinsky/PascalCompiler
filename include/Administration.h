#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <fstream>
#include "Scanner.h"
#include "./Parser.h"

//the maximum number of errors allowed before compilation bails
#define MAX_ERROR_COUNT 10


class Administration
{
public:
    Administration(ifstream& InputFile, ofstream& OutputFile, Scanner* Scan, Parser* Parse);

    /** Call the scanner to scan the input file and extrac tokens */
    void Scan();
    
    /** Call the parser to parse the input tokens */
    void Parse();

private:
    void PrintToken(Token* TokenToPrint) const;

    ofstream& OutFile;
    Scanner* PLScanner;
    Parser* PLParser;

    int ErrorCount;
};


#endif //ADMINISTRATION_H
