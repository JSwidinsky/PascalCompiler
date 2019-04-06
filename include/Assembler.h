// Interface to the assembler class.

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

using namespace std;
#include <iostream>
#include "Parser.h"

class Assembler
{
  public:
   Assembler(istream &in, ostream &out);
   ~Assembler();
   // The two passes of the assembler.
   void firstPass(); 
   void secondPass();

  private:
   int labelTable[MAX_LABELS]; 
   int currentAddress; 
   istream *insource;  // Input file
   ostream *outsource; // Output file 
};
#endif
