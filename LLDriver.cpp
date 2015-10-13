
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


void LLDriver()
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
					int index = findIndex(g.LHS, myStack.top());
					std::vector<string> NTProductions;
					NTProductions = getProductions(g.RHS[index]);
					for(int i = NTProductions.size()-1; i > -1; --i)
					{
						cout << "putting " << NTProductions[i] << " onto the stack " << endl;
						myStack.push(NTProductions[i]);
					}
				}
				else
				{
					cout << "syntax error on " << currentThing << endl;
					//need to advance currentThing I think. Need to do something.
				}
		}
		else //means X is in terminals
		{
			cout << "in else\n";
			if(myStack.top() == currentThing)
			{
				myStack.pop();
				currentThing = "NEXT";//scanner(); //getting next token form user program
			}
			else
			{
				cout << "syntax error on " << currentThing << endl;
				//need way to advance here, just like above
			}
		}
	}
}
