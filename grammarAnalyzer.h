//
//  grammarAnalyzer.hpp
//  asnmt_6_REDO
//
//  Created by Tyler Blanton on 10/5/15.
//  Copyright © 2015 tlblanton. All rights reserved.
//

#ifndef grammarAnalyzer_h
#define grammarAnalyzer_h

#include <iostream>
#include <vector>
using namespace std;

int analyze(string theFile, std::vector<string>&, std::vector<string>&, std::vector<string>&, std::vector<string>&, std::vector<string>&);

bool findItem(std::vector<string> &stringVect, string findThis);

bool existsIn(string thing, std::vector<string> vect);


#endif /* grammarAnalyzer_h */
