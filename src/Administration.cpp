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

Administration::~Administration()
{
    delete PLScanner;
    delete PLParser;
}

void Administration::Compile()
{
    HashTable = new SymbolTable();
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

Token* Administration::GetTokenFromHashTable(const int Index) const
{
    return HashTable->GetTokenAtIndex(Index);
}

void Administration::Emit1(string Opcode)
{
    OutFile << Opcode << endl;
}

void Administration::Emit2(string Opcode, int Value1)
{
    OutFile << Opcode << endl << Value1 << endl;
}

void Administration::Emit3(string Opcode, int Value1, int Value2)
{
    OutFile << Opcode << endl << Value1 << endl << Value2 << endl;
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

void Administration::FatalError(string ErrMessage)
{
    PRINT_ERROR(PLScanner->GetLineNum(), "FATAL --- " + ErrMessage);
    cout << "Compilation aborted." << endl;

    exit(1);
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
        case Symbol::PERIOD: return "'.'"; break;
        case Symbol::COMMA: return "','"; break;
        case Symbol::SEMICOLON: return "';'"; break;
        case Symbol::SQUARELEFT: return "'['"; break;
        case Symbol::SQUARERIGHT: return "']'"; break;
        case Symbol::AND: return "'&'"; break;
        case Symbol::OR: return "'|'"; break;
        case Symbol::NEGATE: return "'~'"; break;
        case Symbol::LESSTHAN: return "'<'"; break;
        case Symbol::EQUAL: return "'='"; break;
        case Symbol::GREATERTHAN: return "'>'"; break;
        case Symbol::PLUS: return "'+'"; break;
        case Symbol::MINUS: return "'-'"; break;
        case Symbol::MULTIPLY: return "'*'"; break;
        case Symbol::DIVIDE: return "'\'"; break;
        case Symbol::MOD: return "'\\'"; break;
        case Symbol::BRACKETLEFT: return "'('"; break;
        case Symbol::BRACKETRIGHT: return "')'"; break;
        case Symbol::ASSIGN: return "':='"; break;
        case Symbol::SUBSCRIPT: return "'[]'"; break;
        case Symbol::ARROW: return "ARROW"; break;
    }
}

bool Administration::IsEmitting() const
{
    return ErrorCount == 0;
}