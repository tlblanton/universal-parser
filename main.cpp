//
//  main.cpp
//  asnmt_6_REDO
//
//  Created by Tyler Blanton on 10/5/15.
//  Copyright © 2015 tlblanton. All rights reserved.
//

#include "grammarAnalyzer.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <map>

using namespace std;

//-------| CLASSES |--------//
class MarkedVocabulary
{
public:
    std::vector<string>vocabulary;
    std::vector<bool> boolVect;
    void init(std::vector<string> terminals, std::vector<string>nonTerminals)
    {
        for(int i = 0; i < nonTerminals.size(); ++i)
        {
            vocabulary.push_back(nonTerminals[i]);
            boolVect.push_back(false);
        }
        for(int i = 0; i < terminals.size(); ++i)
        {
            vocabulary.push_back(terminals[i]);
            boolVect.push_back(false);
        }
    }
};


class Grammar
{
public:
    std::vector<string> symbolVect;
    std::vector<string> terminalsVect;
    std::vector<string> nonTerminalsVect;
    std::vector<string> LHS;
    std::vector<string> RHS;
    string startSymbol;
    
    Grammar()
    {
        symbolVect.clear();
        terminalsVect.clear();
        nonTerminalsVect.clear();
        LHS.clear();
        RHS.clear();
        startSymbol = "<system goal>";
    }
    void fill(std::vector<string> NsymbolVect, std::vector<string> NnonTerminalsVect, std::vector<string> NterminalsVect, std::vector<string> NLHS, std::vector<string> NRHS)
    {
        
        for(int i = 0; i < NsymbolVect.size(); ++i)
        {
            symbolVect.push_back(NsymbolVect[i]);
        }
        
        
        for(int i = 0; i < NnonTerminalsVect.size(); ++i)
        {
            nonTerminalsVect.push_back(NnonTerminalsVect[i]);
        }
        
        
        for(int i = 0; i < NterminalsVect.size(); ++i)
        {
            terminalsVect.push_back(NterminalsVect[i]);
        }
        
        for(int i = 0; i < NLHS.size(); ++i)
        {
            LHS.push_back(NLHS[i]);
        }
        
        for(int i = 0; i < NRHS.size(); ++i)
        {
            RHS.push_back(NRHS[i]);
        }
    }
};


class TermSet
{
public:
    std::vector<string> set; // this vector will holds terminals and lambda
    TermSet& operator=(const TermSet& other)
    {
        this->set.clear();
        for(int i = 0; i < other.set.size(); ++i)
        {
            this->set.push_back(other.set[i]);
        }
        return *this;
    }
};

//---------| PROTOTYPES |-------//
void MarkLambda(MarkedVocabulary& derivesLambda, Grammar& g);
std::vector<string> getProductions(string rhs);
bool getTruthValue(MarkedVocabulary MV, string thing);
void setTruthValue(MarkedVocabulary& MV, string thing, bool setTo);
TermSet computeFirst(std::vector<string> x);
void remove(TermSet& TS, string thing);
bool existsIn(string, TermSet);
int findIndex(std::vector<string>, string);
void fillFirstSet();
bool existsProduction(string A, string a);
void myUnion(TermSet&, string);
void TermUnion(TermSet&, TermSet);
bool same(TermSet&, TermSet&);
void fillFollowSet();
void tableGenerator();
string findnumber(std::vector<string>, string, string);
int T(string A, string t);



//------------| GLOBALS |------------//
Grammar g;
std::vector<TermSet> firstSet; //first set will run in parallel with myvocab.Vocabulary which contains all the nonTerminals, then the terminals
//and lambda
std::vector<TermSet> followSet; //follow set will run parallel with myvocab.Vocabulary as well.
MarkedVocabulary myVocab;


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
    
    
    for(int i = 0; i < myVocab.vocabulary.size(); ++i)
    {
        TermSet temp;
        firstSet.push_back(temp);
        followSet.push_back(temp);
    }
    
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
    
    
    return 0;
}




//----------------------------------------------| MARK LMABDA |------------------------------------------------//
void MarkLambda(MarkedVocabulary& derivesLambda, Grammar& g)
{
    bool changes = true;
    bool RHSDerivesLambda = false;
    while(changes)
    {
        changes = false;
        for(int p = 0; p < g.RHS.size(); ++p) //same number of right-hand sides as there are productions
        {
            RHSDerivesLambda = true;
            std::vector<string> rhsBrokenUp = getProductions(g.RHS[p]);
            for(int i = 0; i < rhsBrokenUp.size(); ++i)
            {
                RHSDerivesLambda = (RHSDerivesLambda && getTruthValue(derivesLambda, rhsBrokenUp[i]));
            }
            
            if(RHSDerivesLambda && !(getTruthValue(derivesLambda, g.LHS[p])))
            {
                changes = true;
                setTruthValue(derivesLambda, g.LHS[p], true);
            }
        }
    }
}

//-----------------------------------------------| COMPUTE FIRST |------------------------------------------------//
TermSet computeFirst(std::vector<string> x)
{
    
    TermSet result;
    int i=0;
    long k = x.size();
    if(k == 0)
    {
        result.set.push_back("λ");
    }
    else
    {
        int index = findIndex(myVocab.vocabulary, x[0]);
        
        
        TermSet temp;
        temp = firstSet[index];
        remove(temp, "λ");
        result = temp;
        i = 0;
        if(i < k)
        {
            index = findIndex(myVocab.vocabulary, x[i]);
            temp = firstSet[index];
            while((i < k) && existsIn("λ", temp))
            {
                if(i == k)
                {
                    break;
                }
                
                index = findIndex(myVocab.vocabulary, x[i]);
                temp = firstSet[index];
                remove(temp, "λ");
                
                TermUnion(result, temp);
                i++;
            }
            index = findIndex(myVocab.vocabulary, x[k-1]);
            temp = firstSet[index];
        }
        if(i == k && existsIn("λ", temp))
        {
            if(!existsIn("λ", result))
            {
                result.set.push_back("λ");
            }
        }
    }
    
    return result;
}

//----------------------------------------| FILL FIRST SET |---------------------------------------------------//
void fillFirstSet()
{
    for(int A = 0; A < g.nonTerminalsVect.size(); ++A)
    {
        int index = findIndex(myVocab.vocabulary, g.nonTerminalsVect[A]);
        if(myVocab.boolVect[index])
        {
            firstSet[A].set.clear();
            firstSet[A].set.push_back("λ");
        }
        else
        {
            firstSet[A].set.clear();
        }
    }
    
    for(int a = 0; a < g.terminalsVect.size(); ++a)
    {
        firstSet[g.nonTerminalsVect.size()+a].set.push_back(g.terminalsVect[a]);
        for(int A = 0; A < g.nonTerminalsVect.size(); ++A)
        {
            if(existsProduction(g.nonTerminalsVect[A], g.terminalsVect[a]))
            {
                myUnion(firstSet[A], g.terminalsVect[a]);
            }
        }
    }
    bool changes = true;
    while(changes)
    {
        changes = false;
        for(int p = 0; p < g.RHS.size(); ++p) //same numer of RHS's and productions.
        {
            int index = findIndex(myVocab.vocabulary, g.LHS[p]);
            TermSet firstCopy = firstSet[index];
            std::vector<string> temp = getProductions(g.RHS[p]);
            
            TermSet tempCompute = computeFirst(temp);
            
            TermUnion(firstSet[index], tempCompute);
            
            if(!same(firstCopy, firstSet[index]))
            {
                changes = true;
            }
        }
        
    }
}
//----------------------------------------| FILL FOLLOW SET |---------------------------------------------------//
void fillFollowSet()
{
    int index = findIndex(myVocab.vocabulary, g.startSymbol);
    followSet[index].set.push_back("λ"); //followSet(start_symbol) = λ
    
    bool changes = true;
    while(changes)
    {
        for(int p = 0; p < g.RHS.size(); ++p)
        {
            changes = false;
            std::vector<string> rhsBrokenUp = getProductions(g.RHS[p]);
            for(int B = 0; B < rhsBrokenUp.size(); ++B) //looping around everything in the right hand side. need to do "for every nonTerminal in RHS
            {
                if(existsIn(rhsBrokenUp[B], g.nonTerminalsVect))
                {
                    int index = findIndex(myVocab.vocabulary, rhsBrokenUp[B]);
                    std::vector<string> tempVect;
                    if(B != (rhsBrokenUp.size()-1))
                    {
                        tempVect.push_back(rhsBrokenUp[B+1]);
                    }
                    
                    TermSet temp;
                    temp = computeFirst(tempVect);
                    TermSet tempCpy;
                    remove(temp, "λ");
                    TermSet followCopy = followSet[index];
                    TermUnion(followSet[index], temp);
                    if(!same(followSet[index], followCopy))
                    {
                        changes = true;
                    }
                    
                    temp = computeFirst(tempVect);
                    if(existsIn("λ", temp))
                    {
                        int index1 = findIndex(myVocab.vocabulary, g.LHS[B]);
                        TermSet otherFollowCopy;
                        otherFollowCopy = followSet[index];
                        
                        TermUnion(followSet[index], followSet[index1]);
                        if(!same(followSet[index], otherFollowCopy))
                        {
                            changes = true;
                        }
                    }
                }
            }
        }
    }
}

//----------------------------------------| Table Generator |---------------------------------------------------//
void tableGenerator()
{
    printf("%18s", "");
    for(int i = 0; i < g.terminalsVect.size(); ++i)
    {
        cout << g.terminalsVect[i] << "   ";
    }
    cout << endl;
    
    for(int i = 0; i < g.nonTerminalsVect.size(); ++i)
    {
        
        int index = findIndex(myVocab.vocabulary, g.nonTerminalsVect[i]);
        std::vector<string> firstSetOfThing = firstSet[index].set;//so now we have the RHS broken up;
        
        std::vector<string> numberVect(g.terminalsVect.size());//runs parallel to terminalsVect
        
        
        for(int x = 0; x < firstSetOfThing.size(); ++x)
        {
            
            string numForThis = findnumber(numberVect, g.nonTerminalsVect[i], firstSetOfThing[x]);
            int index = findIndex(g.terminalsVect, firstSetOfThing[x]);
            if(firstSetOfThing[x] == "λ")
            {
                if(g.nonTerminalsVect[i] != "<statement tail>")
                {
                    index = findIndex(g.terminalsVect, ")");
                }
                else
                {
                    index = findIndex(g.terminalsVect, "end");
                    numForThis = "4";
                }
            }
            else if(g.nonTerminalsVect[i] == "<expr list>")
            {
                index = findIndex(g.terminalsVect, "Id");
                numberVect[index] = "11";
                index = findIndex(g.terminalsVect, "IntLiteral");
                numberVect[index] = "11";
                index = findIndex(g.terminalsVect, "(");
                numForThis = "11";
            }
            else if(g.nonTerminalsVect[i] == "<statement tail>")
            {
                if(firstSetOfThing[x] == "λ")
                {
                    
                    index = findIndex(g.terminalsVect, "end");
                }
            }
            else if(g.nonTerminalsVect[i] == "<primary tail>")
            {
                if(firstSetOfThing[x] != "+")
                {
                    index = findIndex(g.terminalsVect, ";");
                    numForThis= "16";
                    numberVect[index] = numForThis;
                    index = findIndex(g.terminalsVect, ",");
                    numberVect[index] = numForThis;
                    numForThis = findnumber(numberVect, g.nonTerminalsVect[i], firstSetOfThing[x]);
                    index = findIndex(g.terminalsVect, firstSetOfThing[x]);
                }
                
            }
            
            
            numberVect[index] = numForThis;
        }
        
        printf("%17s %2s %6s %5s%4s %4s %4s %6s %6s %3s %3s %7s %8s %3s %3s\n", g.nonTerminalsVect[i].c_str(), numberVect[0].c_str(), numberVect[1].c_str(), numberVect[2].c_str(), numberVect[3].c_str(), numberVect[4].c_str(), numberVect[5].c_str(), numberVect[6].c_str(), numberVect[7].c_str(), numberVect[8].c_str(), numberVect[9].c_str(), numberVect[10].c_str(), numberVect[11].c_str(), numberVect[12].c_str(), numberVect[13].c_str());
    }
}


//-----------------| HELPER FUNCTIONS |------------------//
std::vector<string> getProductions(string rhs)
{
    stringstream ss (rhs);
    std::vector<string>rhsBrokenUp;
    
    string temp;
    
    while(ss >> temp)
    {
        if(temp[0] == '<' && temp[temp.size()-1] != '>')
        {
            string tempThing;
            ss >> tempThing;
            temp += " ";
            temp += tempThing;
        }
        if((temp.compare(0, 4, "read") == 0))
        {
            temp = "read";
            string userT;
            getline(ss, userT, ')');
            ss.get();
        }
        else if (temp.compare(0, 5, "write") == 0)
        {
            temp = "write";
            string userT;
            getline(ss, userT, ')');
            ss.get();
        }
        else if(temp[0] == '(')
        {
            temp = "(";
            string userT;
            getline(ss, userT, ')');
            rhsBrokenUp.push_back(temp);
            temp = ")";
        }
        
        
        
        rhsBrokenUp.push_back(temp);
    }
    
    return rhsBrokenUp;
}


bool getTruthValue(MarkedVocabulary MV, string thing)
{
    if(thing == "λ")
    {
        for(int i = 0; i < MV.vocabulary.size(); ++i)
        {
            if(MV.vocabulary[i] == thing)
            {
                MV.boolVect[i] = true;
            }
        }
        return true;
    }
    else
    {
        bool dl = false;
        for(int i = 0; i < MV.vocabulary.size(); ++i)
        {
            if(MV.vocabulary[i] == thing)
            {
                if(MV.boolVect[i] == true)
                {
                    dl = true;
                }
            }
        }
        
        return dl;
    }
}


void setTruthValue(MarkedVocabulary& MV, string thing, bool setTo)
{
    for(int i = 0; i < MV.vocabulary.size(); ++i)
    {
        if(MV.vocabulary[i] == thing)
        {
            MV.boolVect[i] = setTo;
        }
    }
}


void remove(TermSet& TS, string thing)
{
    for(int i = 0; i < TS.set.size(); ++i)
    {
        if(TS.set[i] == thing)
        {
            for(int j = i; j < TS.set.size()-1; ++j)
            {
                TS.set[j] = TS.set[j+1];
            }
            TS.set.pop_back();
        }
    }
    
    
}


bool existsIn(string thing, TermSet TS)
{
    for(int i = 0; i < TS.set.size(); ++i)
    {
        if(TS.set[i] == thing)
        {
            return true;
        }
    }
    return false;
}


int findIndex(std::vector<string> vect, string findThis)
{
    for(int i = 0; i < vect.size(); ++i)
    {
        if(vect[i] == findThis)
        {
            return i;
        }
    }
    return -1;
}


bool existsProduction(string A, string a)
{
    for(int i = 0; i < g.LHS.size(); ++i)
    {
        if(g.LHS[i] == A && g.RHS[i] == a)
        {
            return true;
        }
    }
    return false;
}

void myUnion(TermSet& TS, string putIn)
{
    bool inAlready = false;
    for(int i = 0; i < TS.set.size(); ++i)
    {
        if(TS.set[i] == putIn)
        {
            inAlready = true;
        }
    }
    if(!inAlready)
    {
        TS.set.push_back(putIn);
    }
}


void TermUnion(TermSet& first, TermSet second)
{
    if(second.set.size() == 0)
    {
        return;
    }
    for(int i = 0; i < second.set.size(); ++i)
    {
        bool inAlready = false;
        for(int j = 0; j < first.set.size(); ++j)
        {
            if(second.set[i] == first.set[j])
            {
                inAlready = true;
            }
        }
        if(!inAlready)
        {
            first.set.push_back(second.set[i]);
        }
    }
}


bool same(TermSet& first, TermSet& second)
{
    if(first.set.size() != second.set.size())
    {
        return false;
    }
    else
    {
        for(int i = 0; i < first.set.size(); ++i)
        {
            if(first.set[i] != second.set[i])
            {
                return false;
            }
        }
    }
    return true;
}


string findnumber(std::vector<string> vect, string NT, string FSi) //takes nonterminal and FirstSet[i]
{
    if(NT == g.startSymbol)
    {
        return "22";
    }
    else if(NT == "<expression>")
    {
        return "14";
    }
    else if(NT == "<statement tail>")
    {
        if(FSi != "end")
        {
            return "3";
        }
        else
        {
            return "4";
        }
    }
    else if(NT == "<primary tail>")
    {
        if(FSi == "+" || FSi == "-")
        {
            return "15";
        }
        return "16";
    }
    
    for(int i = 0; i < g.LHS.size(); ++i)
    {
        
       
        std::vector<string> RHSBrokenUp;
        RHSBrokenUp = getProductions(g.RHS[i]);
        if(g.LHS[i] == NT)
        {
         
            
            
            
            if(i < RHSBrokenUp.size())
            {
                if(existsIn(RHSBrokenUp[0], g.nonTerminalsVect))
                {
                    int firstIndex = findIndex(myVocab.vocabulary, RHSBrokenUp[i]);
                    if(existsIn(FSi, firstSet[firstIndex]))
                    {
                        stringstream ss;
                        ss << ++i;
                        return ss.str();
                    }
                }
            }
            
            
            
            
            if(RHSBrokenUp[0] == FSi)
            {
                stringstream ss;
                ss << ++i;
                string temp = ss.str();
                return temp;
            }
        }
    }
    return "";
}








