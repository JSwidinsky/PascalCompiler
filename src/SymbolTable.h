#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <string>
#include "Token.h"

using namespace std;

/** The table size for our hash table; we pick the first prime number after 300 as our table size */
#define MAX_TABLE_SIZE 307


/**
 * Interface for a hash table, storing an array of tokens
 * Each token is hashed using its lexeme, and is identified in the find function using its lexeme as well
 * Uses standard linear probing to resolve collisions
 */
class SymbolTable
{
public:
    //constructor
    SymbolTable();

    //destructor; deallocates all tokens in the HashTable array
    ~SymbolTable();

    /**
     * Inserts the given token into the hash table
     * Does no internal check to see if the token already exists
     * @param TokenToInsert New token to insert into the table
     * @return The index of the newly inserted token
     */
    int Insert(Token* TokenToInsert);

    /**
     * Finds the given lexeme in the hash table, if it exists
     * @param LexemeToFind The string (lexeme) to find in the hash table
     * @return The index of the token if it is in the hash table, else return -1
     */
    int Find(const string LexemeToFind);

    Token* GetTokenAtIndex(const int Index) const;

    /** Returns if the table is full or not */
    bool IsFull() const;

private:
    /** Add all reserved tokens to the table */
    void InitReserved();

    /**
     * Convert a string into a integer between 0 and MAX_TABLE_SIZE using a hash function
     * @param Lexeme String to convert to an integer
     * @return The index of the hashed string value
     */
    int HashLexeme(string Lexeme) const;

    /** Our hash table array */
    vector<Token*> HashTable;

    /** Number of elements currently in our table */
    int NumElements;
};


#endif //SYMBOLTABLE_H
