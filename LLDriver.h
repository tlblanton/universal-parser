
#ifndef _LLDRIVER_H
#define _LLDRIVER_H

#include "grammarAnalyzer.h"

#include <string>
#include <vector>
#include <stack>

int LLDriver();
void elimFirst(std::vector<string>&);
void display(std::vector<string>&);
void getRemaining(std::vector<string>&);
void displayStack(std::stack<string>);
void displayLine(int, std::vector<string>, std::stack<string>);
std::vector<string> getRealProductions(string rhs);
#endif
