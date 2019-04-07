#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include <vector>

//the maximum number of blocks that we can have stored at any given time
#define MAX_BLOCKS 10

struct TableEntry
{
    enum Kind { CONSTANT, VARIABLE, PROCEDURE, ARRAY };
    enum Type { BOOLEAN, INTEGER, UNIVERSAL };

    TableEntry()
        : Index(0), Size(0), Value(0), Level(0) {}

    TableEntry(int NewIndex, int NewSize, Kind NewKind, Type NewType, int NewVal, int NewLevel, int NewDisplacement, int NewStartLabel)
        : Index(NewIndex), Size(NewSize), MyKind(NewKind), MyType(NewType), Value(NewVal), Level(NewLevel), Displacement(NewDisplacement),
          StartLabel(NewStartLabel) {}
    
    /** Hash table index of the named object */
    int Index;

    /** Size of array, if this is an array (default is zero) */
    int Size;

    /** What kind of entry is this? */
    Kind MyKind;

    /** What is this entry's type? Used for type checking */
    Type MyType;

    /** The value for integer literals */
    int Value;

    /** The level for which block the definition was encountered */
    int Level;

    /** Offset from base address of activation record */
    int Displacement;

    /** Adress of first instruction of a procedure */
    int StartLabel;
};

class BlockTable
{
public:
    BlockTable();
    ~BlockTable();

    bool Search(int Index) const;
    bool Define(TableEntry NewTableEntry);
    TableEntry Find(int Index, bool& WasSuccessful) const;
    bool NewBlock();
    bool PopBlock();
    int GetCurrentLevel() const;

private:
    bool SearchCurrentLevel(int Index) const;

    std::vector<TableEntry> Table[MAX_BLOCKS];
    int CurrentLevel;
};

#endif //BLOCKTABLE_H