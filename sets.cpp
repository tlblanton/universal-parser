/*
 * sets.cpp

 *
 *  Created on: Oct 12, 2015
 *      Author: tlblanton
 */


#include "grammarAnalyzer.h"
#include "sets.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <map>

using namespace std;



extern Grammar g;
extern std::vector<TermSet> firstSet;
extern std::vector<TermSet> followSet;
extern MarkedVocabulary myVocab;
extern std::vector< std::vector<string> > tableVect;

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
void tableGenerator() //need to fill tableVect in here
{
	//initializing the size of tableVect
	tableVect.resize(g.nonTerminalsVect.size());

	for(int d = 0; d < tableVect.size(); ++d)
	{
		tableVect[d].resize(g.terminalsVect.size()); //so every vector has a space for every terminal (to put a potential number there).
	}


    /*printf("%18s", "");
    for(int i = 0; i < g.terminalsVect.size(); ++i)
    {
        cout << g.terminalsVect[i] << "   ";
    }
    cout << endl;
*/

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

                tableVect[i][index] = "11";


                index = findIndex(g.terminalsVect, "IntLiteral");
                numberVect[index] = "11";

                tableVect[i][index] = "11";

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


                    tableVect[i][index] = numForThis;


                    index = findIndex(g.terminalsVect, ",");
                    numberVect[index] = numForThis;

                    tableVect[i][index] = numForThis;

                    numForThis = findnumber(numberVect, g.nonTerminalsVect[i], firstSetOfThing[x]);
                    index = findIndex(g.terminalsVect, firstSetOfThing[x]);
                }

            }


            numberVect[index] = numForThis;


            tableVect[i][index] = numForThis;

        }

        //printf("%17s %2s %6s %5s%4s %4s %4s %6s %6s %3s %3s %7s %8s %3s %3s\n", g.nonTerminalsVect[i].c_str(), numberVect[0].c_str(), numberVect[1].c_str(), numberVect[2].c_str(), numberVect[3].c_str(), numberVect[4].c_str(), numberVect[5].c_str(), numberVect[6].c_str(), numberVect[7].c_str(), numberVect[8].c_str(), numberVect[9].c_str(), numberVect[10].c_str(), numberVect[11].c_str(), numberVect[12].c_str(), numberVect[13].c_str());
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

bool existsInVect(string thing, std::vector<string> vect)
{
    for(int i = 0; i < vect.size(); ++i)
    {
        if(vect[i] == thing)
        {
            if(thing == ";")
            {
                cout << "; is apparently in spot " << i << " in nonterminalsVect" << endl;
            }
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


int T(string X, string a)
{

	int index = findIndex(g.nonTerminalsVect, X);
	int tIndex = findIndex(g.terminalsVect, a);
	if(tableVect[index][tIndex] == "")
	{
		return -1;
	}
	else
	{
		int returnVal = std::stoi(tableVect[index][tIndex], nullptr, 10);
		return returnVal;
	}
}






