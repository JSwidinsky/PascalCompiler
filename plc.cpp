#include <iostream>
#include <fstream>

#include "./src/SymbolTable.h"
#include "./src/Scanner.h"
#include "./src/Administration.h"

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
    Administration* Compiler = new Administration(InputPLFile, OutputExecutable, ScannerObject);
    Compiler->Scan();

    delete Table;
    delete ScannerObject;
    delete Compiler;

    return 0;
}