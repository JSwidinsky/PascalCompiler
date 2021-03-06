#include "../include/Scanner.h"

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;


Scanner::Scanner(ifstream& InputPLFile, SymbolTable* Table)
    : InputPLProgram(InputPLFile)
{
    HashTable = Table;
    LineNum = 1;

    LookAheadSymbol = InputPLProgram.get();
    EOFFlag = false;
}

Scanner::~Scanner()
{
    delete HashTable;
}

Token* Scanner::GetToken()
{
    Token* NewToken = nullptr;
    GetNextSymbol();

    if(!this->AtEndOfFile())
    {
        //remove all whitespace and comments until we reach a valid character
        if(this->IsBlank(CurrentSymbol) || CurrentSymbol == '$')
        {
            RecognizeSeparators();
        }

        if(this->IsAlpha(CurrentSymbol))
        {
            NewToken = this->RecognizeName();
        }
        else if(this->IsNumeric(CurrentSymbol))
        {
            NewToken = this->RecognizeNumeral();
        }
        else if(this->IsSpecial(CurrentSymbol))
        {
            NewToken = this->RecognizeSpecial();
        }
        else
        {
            string ErrMessage = "ERROR LINE: ";
            ErrMessage += to_string(LineNum);
            ErrMessage += " --- Unrecognized character ";
            ErrMessage += CurrentSymbol;

            //remove the rest of the current line we are on, and start fresh at the next line
            //this will be handled better in the next phases
            this->RemoveLine();
            throw runtime_error(ErrMessage);
        }
    }
    else
    {
        NewToken = new Token(Symbol::ENDFILE, TokenAttribute(0, "EOF"));
    }
    

    return NewToken;
}

bool Scanner::AtEndOfFile() const
{
    //return CurrentSymbol == EOF;
    return EOFFlag;
}

int Scanner::GetLineNum() const
{
    return LineNum;
}

bool Scanner::IsAlpha(const char c) const
{
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

bool Scanner::IsNumeric(const char c) const
{
    return ('0' <= c && c <= '9');
}

bool Scanner::IsBlank(const char c) const
{
    return (c == '\t' || c == ' ' || c == '\n');
}

bool Scanner::IsSpecial(const char c) const
{
    switch(c)
    {
        case '.':
        case ',':
        case ';':
        case '[':
        case ']':
        case '&':
        case '|':
        case '~':
        case '<':
        case '=':
        case '>':
        case '+':
        case '-':
        case '*':
        case '/':
        case '\\':
        case '(':
        case ')':
        case ':':
        case '$': return true;
        default: return false;
    }
}

Token* Scanner::RecognizeName()
{
    string Lexeme = "";


    while(this->IsAlpha(CurrentSymbol) || this->IsNumeric(CurrentSymbol) || CurrentSymbol == '_')
    {
        Lexeme += CurrentSymbol;
        GetNextSymbol();
    }

    this->ReverseRead();

    int IndexInTable = HashTable->Find(Lexeme);

    if(IndexInTable != -1)
    {
        //if we already have a token entry in the symbol table, then simply return the already made token
        return HashTable->GetTokenAtIndex(IndexInTable);
    }
    else
    {
        //we must make a new token for the new identifier
        Token* NewToken = new Token(Symbol::ID, TokenAttribute(IndexInTable, Lexeme));
        NewToken->UpdtateValue(HashTable->Insert(NewToken));
        return NewToken;
    }
}

Token* Scanner::RecognizeNumeral()
{
    //we create a value for the integer literal, and make it long long to test for overflow of a 32 bit integer
    long long int Value = 0;
    while(this->IsNumeric(CurrentSymbol))
    {
        Value = (Value * 10) + (static_cast<int>(CurrentSymbol) - '0');

        if(Value > MAX_INT)
        {
            string ErrMessage = "ERROR LINE: ";
            ErrMessage += to_string(LineNum);
            ErrMessage += " --- Size of integer literal exceeds the maximum integer sized allowed (2^32 - 1)";

            //remove the rest of the current line we are on, and start fresh at the next line
            //this will be handled better in the next phases
            this->RemoveLine();
            throw runtime_error(ErrMessage);
        }

        GetNextSymbol();
    }

    if(this->IsAlpha(CurrentSymbol))
    {
        //we have found an invalid identifer, so remove the rest of the line and throw an error
        string ErrMessage = "ERROR LINE: ";
        ErrMessage += to_string(LineNum);
        ErrMessage += " --- Invalid identifier found";

        this->RemoveLine();
        throw runtime_error(ErrMessage);
    }

    this->ReverseRead();

    //make a new token with the value attached to it
    Token* NewToken = new Token(Symbol::NUMERAL, TokenAttribute(Value, ""));
    return NewToken;
}

Token* Scanner::RecognizeSpecial()
{
    string Lexeme = "";

    bool IsLookAheadSpecial = false;

    Token* SpecialToken;

    //is the next symbol potentially part of this operator?
    if(this->IsSpecial(LookAheadSymbol) && LookAheadSymbol != '$')
    {
        IsLookAheadSpecial = true;
        if(CurrentSymbol == '[' && LookAheadSymbol == ']')  //subscript operator
        {
            Lexeme += CurrentSymbol;
            Lexeme += LookAheadSymbol;
            SpecialToken = new Token(Symbol::SUBSCRIPT, TokenAttribute(-1, Lexeme));
        }
        else if(CurrentSymbol == ':' && LookAheadSymbol == '=')  //assignment operator
        {
            Lexeme += CurrentSymbol;
            Lexeme += LookAheadSymbol;
            SpecialToken = new Token(Symbol::ASSIGN, TokenAttribute(-1, Lexeme));
        }
        else if(CurrentSymbol == '-' && LookAheadSymbol == '>')  //arrow operator
        {
            Lexeme += CurrentSymbol;
            Lexeme += LookAheadSymbol;
            SpecialToken = new Token(Symbol::ARROW, TokenAttribute(-1, Lexeme));
        }
        else
        {
            IsLookAheadSpecial = false;
        }
    }

    //make a new token with the proper symbol type attached to it
    if(!IsLookAheadSpecial)
    {
        Lexeme = CurrentSymbol;

        switch(CurrentSymbol)
        {
            case '.': SpecialToken = new Token(Symbol::PERIOD, TokenAttribute(-1, Lexeme)); break;
            case ',': SpecialToken = new Token(Symbol::COMMA, TokenAttribute(-1, Lexeme)); break;
            case ';': SpecialToken = new Token(Symbol::SEMICOLON, TokenAttribute(-1, Lexeme)); break;
            case '[': SpecialToken = new Token(Symbol::SQUARELEFT, TokenAttribute(-1, Lexeme)); break;
            case ']': SpecialToken = new Token(Symbol::SQUARERIGHT, TokenAttribute(-1, Lexeme)); break;
            case '&': SpecialToken = new Token(Symbol::AND, TokenAttribute(-1, Lexeme)); break;
            case '|': SpecialToken = new Token(Symbol::OR, TokenAttribute(-1, Lexeme)); break;
            case '~': SpecialToken = new Token(Symbol::NEGATE, TokenAttribute(-1, Lexeme)); break;
            case '<': SpecialToken = new Token(Symbol::LESSTHAN, TokenAttribute(-1, Lexeme)); break;
            case '=': SpecialToken = new Token(Symbol::EQUAL, TokenAttribute(-1, Lexeme)); break;
            case '>': SpecialToken = new Token(Symbol::GREATERTHAN, TokenAttribute(-1, Lexeme)); break;
            case '+': SpecialToken = new Token(Symbol::PLUS, TokenAttribute(-1, Lexeme)); break;
            case '-': SpecialToken = new Token(Symbol::MINUS, TokenAttribute(-1, Lexeme)); break;
            case '*': SpecialToken = new Token(Symbol::MULTIPLY, TokenAttribute(-1, Lexeme)); break;
            case '/': SpecialToken = new Token(Symbol::DIVIDE, TokenAttribute(-1, Lexeme)); break;
            case '\\': SpecialToken = new Token(Symbol::MOD, TokenAttribute(-1, Lexeme)); break;
            case '(': SpecialToken = new Token(Symbol::BRACKETLEFT, TokenAttribute(-1, Lexeme)); break;
            case ')': SpecialToken = new Token(Symbol::BRACKETRIGHT, TokenAttribute(-1, Lexeme)); break;
        }
    }
    else
    {
        GetNextSymbol();
    }

    return SpecialToken;
}

void Scanner::RecognizeSeparators()
{
    //remove blanks, and if we find a comment, remove the entire line
    while(this->IsBlank(CurrentSymbol) || CurrentSymbol == '$')
    {
        if(CurrentSymbol == '$')
        {
            this->RemoveLine();
        }
        else if(this->IsBlank(CurrentSymbol))
        {
            if(CurrentSymbol == '\n')
            {
                ++LineNum;
            }
        }
        GetNextSymbol();
    }
}

void Scanner::GetNextSymbol()
{
    if(LookAheadSymbol == EOF)
    {
        EOFFlag = true;
    }
    
    CurrentSymbol = LookAheadSymbol;
    LookAheadSymbol = InputPLProgram.get();
}

void Scanner::ReverseRead()
{
    InputPLProgram.putback(LookAheadSymbol);
    LookAheadSymbol = CurrentSymbol;
}

void Scanner::RemoveLine()
{
    ++LineNum;
    while(CurrentSymbol != '\n')
    {
        GetNextSymbol();
    }
}