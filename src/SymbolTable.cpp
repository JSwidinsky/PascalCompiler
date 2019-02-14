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
    for(int i = 0; i < HashTable.size(); ++i)
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
    TokenAttribute Attr;

    Attr.Lexeme = "begin";
    this->Insert(new Token(Symbol::BEGIN, Attr));

    Attr.Lexeme = "end";
    this->Insert(new Token(Symbol::END, Attr));

    Attr.Lexeme = "const";
    this->Insert(new Token(Symbol::CONST, Attr));

    Attr.Lexeme = "array";
    this->Insert(new Token(Symbol::ARRAY, Attr));

    Attr.Lexeme = "integer";
    this->Insert(new Token(Symbol::INTEGER, Attr));

    Attr.Lexeme = "Boolean";
    this->Insert(new Token(Symbol::BOOLEAN, Attr));

    Attr.Lexeme = "proc";
    this->Insert(new Token(Symbol::PROC, Attr));

    Attr.Lexeme = "skip";
    this->Insert(new Token(Symbol::SKIP, Attr));

    Attr.Lexeme = "read";
    this->Insert(new Token(Symbol::READ, Attr));

    Attr.Lexeme = "write";
    this->Insert(new Token(Symbol::WRITE, Attr));

    Attr.Lexeme = "call";
    this->Insert(new Token(Symbol::CALL, Attr));

    Attr.Lexeme = "if";
    this->Insert(new Token(Symbol::IF, Attr));

    Attr.Lexeme = "do";
    this->Insert(new Token(Symbol::DO, Attr));

    Attr.Lexeme = "fi";
    this->Insert(new Token(Symbol::FI, Attr));

    Attr.Lexeme = "od";
    this->Insert(new Token(Symbol::OD, Attr));

    Attr.Lexeme = "false";
    this->Insert(new Token(Symbol::FALSE, Attr));

    Attr.Lexeme = "true";
    this->Insert(new Token(Symbol::TRUE, Attr));
}

int SymbolTable::HashLexeme(string Lexeme) const
{
    int HashValue = 0;

    //here is the hash function (it is a simple hash function)
    //take the ASCII value of the character and add it to our current hash value
    for(int i = 0; i < Lexeme.size(); i++)
    {
        HashValue = (HashValue + Lexeme[i]) % MAX_TABLE_SIZE;
    }

    return HashValue;
}