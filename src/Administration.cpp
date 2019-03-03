#include "../include/Administration.h"
#include "../include/SymbolTable.h"
#include <iostream>
#include <stdexcept>
#include <queue>
using namespace std;

//simple macro to print an error (on line n) with string message s
#define PRINT_ERROR(n, s) cerr << "ERROR LINE " << n << ": " << s << endl


Administration::Administration(ifstream& InputFile, ofstream& OutputFile)
    : InFile(InputFile), OutFile(OutputFile)
{
    ErrorCount = 0;
}

void Administration::Compile()
{
    SymbolTable* HashTable = new SymbolTable();
    PLScanner = new Scanner(InFile, HashTable);

    PLParser = new Parser(this);

    PLParser->BeginParsing();

    cout << "ERROR COUNT: " << ErrorCount << endl;
}

Token* Administration::GetNextToken()
{
    while (true)
    {
        try
        {
            Token* TokenToAdd = PLScanner->GetToken();

            return TokenToAdd;
        }
        catch (runtime_error &err)
        {
            ReportError(err.what());
        }
    }
}

void Administration::ReportError(string ErrMessage)
{
    PRINT_ERROR(PLScanner->GetLineNum(), ErrMessage);

    ++ErrorCount;
    if(ErrorCount > MAX_ERROR_COUNT)
    {
        cout << "Error count exceeds " << MAX_ERROR_COUNT << ". Compilation aborted." << endl;
        exit(1);
    }
}

string Administration::TokenToString(const Symbol::Symbol symbol)
{
    switch(symbol)
    {
        case Symbol::ENDFILE: return "ENDFILE"; break;
        case Symbol::BEGIN: return "BEGIN"; break;
        case Symbol::END: return "END"; break;
        case Symbol::CONST: return "CONST"; break;
        case Symbol::ARRAY: return "ARRAY"; break;
        case Symbol::INTEGER: return "INTEGER"; break;
        case Symbol::BOOLEAN: return "BOOLEAN"; break;
        case Symbol::PROC: return "PROC"; break;
        case Symbol::SKIP: return "SKIP"; break;
        case Symbol::READ: return "READ"; break;
        case Symbol::WRITE: return "WRITE"; break;
        case Symbol::CALL: return "CALL"; break;
        case Symbol::IF: return "IF"; break;
        case Symbol::DO: return "DO"; break;
        case Symbol::FI: return "FI"; break;
        case Symbol::OD: return "OD"; break;
        case Symbol::FALSE: return "FALSE"; break;
        case Symbol::TRUE: return "TRUE"; break;
        case Symbol::ID: return "ID "; break;
        case Symbol::NUMERAL: return "NUMERAL "; break;
        case Symbol::PERIOD: return "PERIOD"; break;
        case Symbol::COMMA: return "COMMA"; break;
        case Symbol::SEMICOLON: return "SEMICOLON"; break;
        case Symbol::SQUARELEFT: return "SQUARELEFT"; break;
        case Symbol::SQUARERIGHT: return "SQUARERIGHT"; break;
        case Symbol::AND: return "AND"; break;
        case Symbol::OR: return "OR"; break;
        case Symbol::NEGATE: return "NEGATE"; break;
        case Symbol::LESSTHAN: return "LESSTHAN"; break;
        case Symbol::EQUAL: return "EQUAL"; break;
        case Symbol::GREATERTHAN: return "GREATERTHAN"; break;
        case Symbol::PLUS: return "PLUS"; break;
        case Symbol::MINUS: return "MINUS"; break;
        case Symbol::MULTIPLY: return "MULTIPLY"; break;
        case Symbol::DIVIDE: return "DIVIDE"; break;
        case Symbol::MOD: return "MOD"; break;
        case Symbol::BRACKETLEFT: return "BRACKETLEFT"; break;
        case Symbol::BRACKETRIGHT: return "BRACKETRIGHT"; break;
        case Symbol::ASSIGN: return "ASSIGN"; break;
        case Symbol::SUBSCRIPT: return "SUBSCRIPT"; break;
        case Symbol::ARROW: return "ARROW"; break;
    }
}