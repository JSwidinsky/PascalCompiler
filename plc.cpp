#include <iostream>
#include <fstream>

#include "./include/SymbolTable.h"
#include "./include/Scanner.h"
#include "./include/Administration.h"
#include "./include/Parser.h"
#include "./include/Assembler.h"

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

    ofstream TempOutput("./temp.txt");
    Administration* Compiler = new Administration(InputPLFile, TempOutput);
    Compiler->Compile();
    TempOutput.close();

    ifstream AssemblerInput("./temp.txt");
    Assembler* AssemblerObject;
    if(AssemblerInput.good() && Compiler->IsEmitting())
    {
        AssemblerObject = new Assembler(AssemblerInput, OutputExecutable);
        AssemblerObject->firstPass();
        AssemblerInput.seekg(0);  //reset the head of the input file to the beginning of the file
        AssemblerObject->secondPass();

        delete AssemblerObject;
        remove("./temp.txt");
    }

    delete Compiler;
    return 0;
}