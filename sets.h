/*
 * sets.h

 *
 *  Created on: Oct 12, 2015
 *      Author: tlblanton
 */

#ifndef SETS_H_
#define SETS_H_


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
bool existsInVect(string thing, std::vector<string> vect);
int T(string A, string t); //A is a nonterminal and t is a look-ahead token





#endif /* SETS_H_ */
