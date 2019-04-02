#include "../include/SymbolTable.h"

#include <iostream>
using namespace std;

SymbolTable::SymbolTable()
{
    HashTable.resize(MAX_TABLE_SIZE);
    NumElements = 0;

    this->InitReserved();
}

SymbolTable::~SymbolTable()
{
    for(unsigned int i = 0; i < HashTable.size(); ++i)
        if(HashTable[i])
            delete HashTable[i];
}

int SymbolTable::Insert(Token* TokenToInsert)
{
    int HashedIndex = this->HashLexeme(TokenToInsert->GetLexeme());

    //increase the hashed index while we detect collision
    //(i.e. while the entry in the hash table is already occupied, search the next spot)
    while(HashTable[HashedIndex] && !this->IsFull())
    {
        HashedIndex = (HashedIndex + 1) % MAX_TABLE_SIZE;
    }

    HashTable[HashedIndex] = TokenToInsert;

    ++NumElements;

    return HashedIndex;
}

int SymbolTable::Find(const string LexemeToFind)
{
    int HashedIndex = this->HashLexeme(LexemeToFind);

    while(HashTable[HashedIndex])
    {
        if(LexemeToFind.compare(HashTable[HashedIndex]->GetLexeme()) == 0)
            return HashedIndex;
        else
            HashedIndex = (HashedIndex + 1) % MAX_TABLE_SIZE;
    }

    //we have reached an empty spot and did not find our lexeme, so the token doesn't exist in the table
    return -1;
}

Token* SymbolTable::GetTokenAtIndex(const int Index) const
{
    return HashTable[Index];
}

bool SymbolTable::IsFull() const
{
    return NumElements == MAX_TABLE_SIZE;
}

void SymbolTable::InitReserved()
{
    string Lexeme;

    Lexeme = "begin";
    this->Insert(new Token(Symbol::BEGIN, TokenAttribute(-1, Lexeme)));

    Lexeme = "end";
    this->Insert(new Token(Symbol::END, TokenAttribute(-1, Lexeme)));

    Lexeme = "const";
    this->Insert(new Token(Symbol::CONST, TokenAttribute(-1, Lexeme)));

    Lexeme = "array";
    this->Insert(new Token(Symbol::ARRAY, TokenAttribute(-1, Lexeme)));

    Lexeme = "integer";
    this->Insert(new Token(Symbol::INTEGER, TokenAttribute(-1, Lexeme)));

    Lexeme = "Boolean";
    this->Insert(new Token(Symbol::BOOLEAN, TokenAttribute(-1, Lexeme)));

    Lexeme = "proc";
    this->Insert(new Token(Symbol::PROC, TokenAttribute(-1, Lexeme)));

    Lexeme = "skip";
    this->Insert(new Token(Symbol::SKIP, TokenAttribute(-1, Lexeme)));

    Lexeme = "read";
    this->Insert(new Token(Symbol::READ, TokenAttribute(-1, Lexeme)));

    Lexeme = "write";
    this->Insert(new Token(Symbol::WRITE, TokenAttribute(-1, Lexeme)));

    Lexeme = "call";
    this->Insert(new Token(Symbol::CALL, TokenAttribute(-1, Lexeme)));

    Lexeme = "if";
    this->Insert(new Token(Symbol::IF, TokenAttribute(-1, Lexeme)));

    Lexeme = "do";
    this->Insert(new Token(Symbol::DO, TokenAttribute(-1, Lexeme)));

    Lexeme = "fi";
    this->Insert(new Token(Symbol::FI, TokenAttribute(-1, Lexeme)));

    Lexeme = "od";
    this->Insert(new Token(Symbol::OD, TokenAttribute(-1, Lexeme)));

    Lexeme = "false";
    this->Insert(new Token(Symbol::FALSE, TokenAttribute(-1, Lexeme)));

    Lexeme = "true";
    this->Insert(new Token(Symbol::TRUE, TokenAttribute(-1, Lexeme)));
}

int SymbolTable::HashLexeme(string Lexeme) const
{
    int HashValue = 0;

    //here is the hash function (it is a simple hash function)
    //take the ASCII value of the character and add it to our current hash value
    for(const char& c : Lexeme)
    {
        HashValue = (HashValue + c) % MAX_TABLE_SIZE;
    }

    return HashValue;
}