//
//  main.cpp
//  asnmt_6_REDO
//
//  Created by Tyler Blanton on 10/5/15.
//  Copyright © 2015 tlblanton. All rights reserved.
//

#include "grammarAnalyzer.h"
#include "sets.h"
#include "LLDriver.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <map>
#include <stack>

using namespace std;


//------------| GLOBALS |------------//
Grammar g;
std::vector<TermSet> firstSet; //first set will run in parallel with myvocab.Vocabulary which contains all the nonTerminals, then the terminals
//and lambda
std::vector<TermSet> followSet; //follow set will run parallel with myvocab.Vocabulary as well.
MarkedVocabulary myVocab;
std::vector<std::vector <string> > tableVect; ;//this runs parallel to the production numbers and therefore also LHS
std::stack<string> myStack;

//----------------------------------------| MAIN |------------------------------------------//
int main()
{
    std::vector<string>* globsymbolVect;
    std::vector<string>* globnonTerminalsVect;
    std::vector<string>* globterminalsVect;
    std::vector<string>* globLHS;
    std::vector<string>* globRHS;
    globRHS = new std::vector<string>;
    globsymbolVect = new std::vector<string>;
    globnonTerminalsVect = new std::vector<string>;
    globterminalsVect = new std::vector<string>;
    globLHS = new std::vector<string>;
    
    int status = analyze("microProductions.txt", *globsymbolVect, *globnonTerminalsVect, *globterminalsVect, *globLHS, *globRHS);
    if(status == -1)
    {
        cerr << "File specified could not be opened. Program Terminated" << endl;
        return -1;
    }
    g.fill(*globsymbolVect, *globnonTerminalsVect, *globterminalsVect, *globLHS, *globRHS);
    myVocab.init(g.terminalsVect, g.nonTerminalsVect);
    myVocab.vocabulary.push_back("λ");
    
    
    
    
    
    delete globsymbolVect;
    delete globterminalsVect;
    delete globnonTerminalsVect;
    delete globLHS;
    delete globRHS;



   MarkLambda(myVocab, g);

    firstSet.resize(myVocab.vocabulary.size()); 	//giving first and follow sets the correct size
    followSet.resize(myVocab.vocabulary.size());
    
    fillFirstSet();

    fillFollowSet();

    /*
    for(int i = 0; i < g.nonTerminalsVect.size(); ++i)
    {
        //cout << "first set of " << myVocab.vocabulary[i] << setw(30) << " is {  ";
        printf("first set of %-30s { ", myVocab.vocabulary[i].c_str());
        for(int j = 0; j < firstSet[i].set.size(); ++j)
        {
            if(j == (firstSet[i].set.size()-1))
            {
                cout << firstSet[i].set[j] << " }";
            }
            else
            {
                cout <<firstSet[i].set[j] << " ";
            }
        }
        
        cout  <<endl;
    }
*/
    /*
    cout << "---------------------------------------------------" << endl;
    for(int i = 0; i < g.nonTerminalsVect.size(); ++i)
    {
        //cout << "follow set of " << myVocab.vocabulary[i] << setw(30) << " is {  ";
        printf("follow set of %-29s { ", myVocab.vocabulary[i].c_str());
        for(int j = 0; j < followSet[i].set.size(); ++j)
        {
            if(j == (followSet[i].set.size()-1))
            {
                cout << followSet[i].set[j] << " }";
            }
            else
            {
                cout <<followSet[i].set[j] << " ";
            }
        }
        
        cout  <<endl;
    }
    */

    cout << "------------------------------------------------------------------------------------------------" << endl;
    

    tableGenerator();
    if(LLDriver() == -1)
    {
        cerr << "Program Terminated." << endl;
        return EXIT_FAILURE;
    }


    return 0;
}


