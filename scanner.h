
#ifndef _SCANNER_H
#define _SCANNER_H

#include <iostream>
#include "sets.h"
#include "grammarAnalyzer.h"
#include "LLDriver.h"

string scanner(int optional = 0); //returns Token. called in loop. when any non-zero number is passed to it, the next token is
//returned but the stream is not advanced. works like a peek
void bufferChar(char currentChar, string& singleTokenBuffer); //adds character to tokenBuffer
void clearBuffer(string& tokenBuffer); //clears tokenBuffer
char inspect(ifstream& inFile); //peeks at next char
string checkReserved(string input); //takes identifiers, returns strings that are tokens.
void advance(ifstream& inFile);//moves forward one character
char read(ifstream& inFile);//reads next character




#endif
