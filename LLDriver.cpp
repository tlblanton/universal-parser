
#include "LLDriver.h"
#include "grammarAnalyzer.h"
#include "sets.h"
#include <string>
#include <stack>
using namespace std;

extern std::stack<string> myStack;
extern Grammar g;
extern std::vector<TermSet> firstSet;
extern std::vector<TermSet> followSet;
extern MarkedVocabulary myVocab;
extern std::vector< std::vector<string> > tableVect;


void LLDrriver()
{
	string currentThing;
	currentThing = "TEST";//scanner(); //need to implemet scanner to read program. use other one?
	myStack.push(g.startSymbol);
	while(myStack.size() != 0)
	{
		if(existsInVect(myStack.top(), g.nonTerminalsVect))
		{
				if(T(myStack.top(), currentThing) != -1)
				{

				}
				else
				{
					cout << "syntax error on " << currentThing << endl;
				}
		}
	}


}
