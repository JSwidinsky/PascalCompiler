#include "../include/BlockTable.h"

#include <iostream>
using namespace std;

BlockTable::BlockTable()
    : CurrentLevel(-1) {}

BlockTable::~BlockTable()
{}

bool BlockTable::Search(int Index) const
{
    //perform a linear scan through all the levels we have defined so far in our Table array
    //this is not the most efficient way, but it does the job
    for(int i = CurrentLevel; i >= 0; --i)
    {
        for(const TableEntry& CurrentEntry : Table[i])
        {
            if(CurrentEntry.Index == Index)
            {
                return true;
            }
        }
    }
    return false;
}

bool BlockTable::Define(TableEntry NewTableEntry)
{
    //return false if our new table entry already exists in our table
    if(SearchCurrentLevel(NewTableEntry.Index))
    {
        return false;
    }

    Table[CurrentLevel].push_back(NewTableEntry);
    return true;
}

TableEntry BlockTable::Find(int Index, bool& WasSuccessful) const
{
    TableEntry TableToReturn;
    for(int i = CurrentLevel; i >= 0; --i)
    {
        for(const TableEntry& CurrentEntry : Table[i])
        {
            if(CurrentEntry.Index == Index)
            {
                WasSuccessful = true;
                return TableToReturn = CurrentEntry;
            }
        }
    }
    WasSuccessful = false;
    return TableToReturn;
}

bool BlockTable::NewBlock()
{
    ++CurrentLevel;

    //don't add a new block if we already have the max allowed blocks
    if(CurrentLevel >= MAX_BLOCKS)
    {
        --CurrentLevel;
        return false;
    }

    return true;
}

bool BlockTable::PopBlock()
{
    //TODO: Think of the logic for this function; it may not work
    //don't pop our block if we already have nothing in the table
    if(CurrentLevel < 0)
    {
        return false;
    }

    --CurrentLevel;

    Table[CurrentLevel + 1].clear();
    return true;
}

int BlockTable::GetCurrentLevel() const
{
    return CurrentLevel;
}

bool BlockTable::SearchCurrentLevel(int Index) const
{
    //perform a linear scan through all the levels we have defined so far in our Table array
    //this is not the most efficient way, but it does the job
    for(const TableEntry& CurrentEntry : Table[CurrentLevel])
    {
        if(CurrentEntry.Index == Index)
        {
            return true;
        }
    }
    return false;
}