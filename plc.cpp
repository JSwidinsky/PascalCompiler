#include <iostream>
#include <fstream>

#include "./include/SymbolTable.h"
#include "./include/Scanner.h"
#include "./include/Administration.h"
#include "./include/Parser.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "Invalid input" << endl;
        return 0;
    }

    ifstream InputPLFile(argv[1]);
    if(!InputPLFile)
    {
        std::cout << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    ofstream OutputExecutable(argv[2]);
    if(!OutputExecutable)
    {
        cout << "Could not open output file " << argv[2] << endl;
        return 1;
    }

    SymbolTable* Table = new SymbolTable();
    Scanner* ScannerObject = new Scanner(InputPLFile, Table);
    Parser* ParserObject = new Parser();
    Administration* Compiler = new Administration(InputPLFile, OutputExecutable, ScannerObject, ParserObject);
    Compiler->Scan();
    Compiler->Parse();

    delete Table;
    delete ScannerObject;
    delete Compiler;
    delete ParserObject;

    return 0;
}