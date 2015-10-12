//
//  grammarAnalyzer.cpp
//  asnmt_6_REDO
//
//  Created by Tyler Blanton on 10/5/15.
//  Copyright © 2015 tlblanton. All rights reserved.
//

#include "grammarAnalyzer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include<sstream>
using namespace std;



//string theFile = "microProductions.txt";

int analyze(string theFile, std::vector<string>& symbolVect, std::vector<string>& nonTerminalsVect, std::vector<string>& terminalsVect, std::vector<string>& LHS, std::vector<string>& RHS)
{
    static int i = 0;
    static ifstream inFile;
    if(i == 0)
    {
        inFile.open(theFile);
        if(!inFile)
        {
            return -1;
        }
        i++;
    }
    string currentThing;
    
    while(inFile.peek() != EOF)
    {
        inFile >> currentThing;
        
        
        while(currentThing[0] == '<' && currentThing[currentThing.size()-1] != '>')
        {
            string tempThing;
            inFile >> tempThing;
            currentThing += " ";
            currentThing += tempThing;
        }
        
        while(currentThing[0] =='(' && currentThing[currentThing.size()-1] != ')')
        {
            string temp;
            inFile >> temp;
            currentThing += " ";
            currentThing += temp;
        }

        
        if(currentThing[0] == '<' && currentThing[currentThing.size()-1] == '>' && currentThing != "->")
        {
            if(!findItem(nonTerminalsVect, currentThing))
            {
                nonTerminalsVect.push_back(currentThing);
            }
            if(!findItem(symbolVect, currentThing))
            {
                symbolVect.push_back(currentThing);
            }
        }
        else if(currentThing[0] == '(')
        {
            //string temp;
            //getline(inFile, temp, ')');
            if(!existsIn("(", terminalsVect))
            {
                terminalsVect.push_back("(");
            }
            if(!existsIn(")", terminalsVect))
            {
                terminalsVect.push_back(")");
            }
        }
        else if(currentThing != "->" && !findItem(terminalsVect, currentThing))
        {
            if(currentThing.compare(0, 4, "read") == 0)
            {
                currentThing = "read";
                string temp;
                getline(inFile, temp, ')'); //read up until ")"
                inFile.get(); //consume RParen
            }
            else if(currentThing.compare(0, 6, "write(") == 0)
            {
                currentThing = "write";
                string temp;
                getline(inFile, temp, ')'); //read up until ")"
                inFile.get(); //consume RParen
            }
            if(currentThing != "λ")
            {
                terminalsVect.push_back(currentThing);
            }
            if(!findItem(symbolVect, currentThing))
            {
                symbolVect.push_back(currentThing);
            }
        }
        
    }
    inFile.close();
    
    inFile.open(theFile);
    
    while(inFile.peek() != EOF)
    {
        inFile >> currentThing;
        while(currentThing[0] == '<' && currentThing[currentThing.size()-1] != '>')
        {
            string tempThing;
            inFile >> tempThing;
            currentThing += " ";
            currentThing += tempThing;
        }
        LHS.push_back(currentThing);
        
        inFile >> currentThing; //this consumes the "->"
        
        getline(inFile, currentThing, '\n');
        RHS.push_back(currentThing);
    }
    
    
    
    return 0;
}


bool findItem(std::vector<string> &stringVect, string findThis)
{
    bool found = false;
    for(int i = 0; i < stringVect.size(); ++i)
    {
        if(stringVect[i] == findThis)
        {
            found = true;
        }
    }
    return found;
}



bool existsIn(string thing, std::vector<string> vect)
{
    for(int i = 0; i < vect.size(); ++i)
    {
        if(vect[i] == thing)
        {
            return true;
        }
    }
    return false;
}


