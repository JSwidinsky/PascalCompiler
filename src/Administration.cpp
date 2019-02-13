#include "Administration.h"

#include <iostream>
#include <exception>
using namespace std;

Administration::Administration(ifstream& InputFile, ofstream& OutputFile, Scanner* Scan)
    : OutFile(OutputFile)
{
    PLScanner = Scan;

    ErrorCount = 0;
}

void Administration::Scan()
{
    while (!PLScanner->AtEndOfFile())
    {
        try
        {
            Token* NextToken = PLScanner->GetToken();

            this->PrintToken(NextToken);

            //for now, delete the token if it is not in the symbol table
            //this will change in the later phases, but for now it eliminates the memory leaks
            switch(NextToken->GetSymbolName())
            {
                case Symbol::BEGIN:
                case Symbol::END:
                case Symbol::CONST:
                case Symbol::ARRAY:
                case Symbol::INTEGER:
                case Symbol::BOOLEAN:
                case Symbol::PROC:
                case Symbol::SKIP:
                case Symbol::READ:
                case Symbol::WRITE:
                case Symbol::CALL:
                case Symbol::IF:
                case Symbol::DO:
                case Symbol::FI:
                case Symbol::OD:
                case Symbol::FALSE:
                case Symbol::TRUE:
                case Symbol::ID:break;
                default: delete NextToken; break;
            }
        }
        catch (runtime_error &err)
        {
            //we have caught an error
            ++ErrorCount;

            cerr << err.what() << endl;

            //bail on compilation if the error count exceeds the maximum allowed errors
            if(ErrorCount > MAX_ERROR_COUNT)
            {
                cout << "Error count exceeds " << MAX_ERROR_COUNT << ". Compilation aborted." << endl;
                exit(1);
            }
        }
    }

    cout << "\nNum errors detected: " << ErrorCount << endl;
}

void Administration::PrintToken(Token* TokenToPrint) const
{
    OutFile << "Token Symbol: ";

    //this is a pretty horrid way to print the enum name, but it works for now
    switch(TokenToPrint->GetSymbolName())
    {
        case Symbol::BEGIN: OutFile << "BEGIN" << endl; break;
        case Symbol::END: OutFile << "END" << endl; break;
        case Symbol::CONST: OutFile << "CONST" << endl; break;
        case Symbol::ARRAY: OutFile << "ARRAY" << endl; break;
        case Symbol::INTEGER: OutFile << "INTEGER" << endl; break;
        case Symbol::BOOLEAN: OutFile << "BOOLEAN" << endl; break;
        case Symbol::PROC: OutFile << "PROC" << endl; break;
        case Symbol::SKIP: OutFile << "SKIP" << endl; break;
        case Symbol::READ: OutFile << "READ" << endl; break;
        case Symbol::WRITE: OutFile << "WRITE" << endl; break;
        case Symbol::CALL: OutFile << "CALL" << endl; break;
        case Symbol::IF: OutFile << "IF" << endl; break;
        case Symbol::DO: OutFile << "DO" << endl; break;
        case Symbol::FI: OutFile << "FI" << endl; break;
        case Symbol::OD: OutFile << "OD" << endl; break;
        case Symbol::FALSE: OutFile << "FALSE" << endl; break;
        case Symbol::TRUE: OutFile << "TRUE" << endl; break;
        case Symbol::ID: OutFile << "ID " << TokenToPrint->GetLexeme() << endl; break;
        case Symbol::NUMERAL: OutFile << "NUMERAL" << endl; break;
        case Symbol::PERIOD: OutFile << "PERID" << endl; break;
        case Symbol::COMMA: OutFile << "COMMA" << endl; break;
        case Symbol::SEMICOLON: OutFile << "SEMICOLON" << endl; break;
        case Symbol::SQUARELEFT: OutFile << "SQUARELEFT" << endl; break;
        case Symbol::SQUARERIGHT: OutFile << "SQUARERIGHT" << endl; break;
        case Symbol::AND: OutFile << "AND" << endl; break;
        case Symbol::OR: OutFile << "OR" << endl; break;
        case Symbol::NEGATE: OutFile << "NEGATE" << endl; break;
        case Symbol::LESSTHAN: OutFile << "LESSTHAN" << endl; break;
        case Symbol::EQUAL: OutFile << "EQUAL" << endl; break;
        case Symbol::GREATERTHAN: OutFile << "GREATERTHAN" << endl; break;
        case Symbol::PLUS: OutFile << "PLUS" << endl; break;
        case Symbol::MINUS: OutFile << "MINUS" << endl; break;
        case Symbol::MULTIPLY: OutFile << "MULTIPLY" << endl; break;
        case Symbol::DIVIDE: OutFile << "DIVIDE" << endl; break;
        case Symbol::MOD: OutFile << "MOD" << endl; break;
        case Symbol::BRACKETLEFT: OutFile << "BRACKETLEFT" << endl; break;
        case Symbol::BRACKETRIGHT: OutFile << "BRACKETRIGHT" << endl; break;
        case Symbol::ASSIGN: OutFile << "ASSIGN" << endl; break;
        case Symbol::SUBSCRIPT: OutFile << "SUBSCRIPT" << endl; break;
        case Symbol::ARROW: OutFile << "ARROW" << endl; break;
    }


}
