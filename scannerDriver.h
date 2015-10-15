//
// Created by Tyler Blanton on 10/15/15.
//

#ifndef _SCANNERDRIVER_H
#define _SCANNERDRIVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <ctype.h>
using namespace std;

void scannerDriver(int& tokenCode, string& tokenText);
string action(int state, char currentChar);//will return moveAppend, moveNoappend, one of those
int nextState(int state, char currentChar);
void consumeChar();
void lookupCode(int state, char currentChar, int &tokenCode);
void checkExceptions(int &tokenCode, string &tokenText);
char currentChar();


#endif //UNIVERSAL_PARSER_SCANNERDRIVER_H
