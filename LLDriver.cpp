
#include "LLDriver.h"
#include "grammarAnalyzer.h"
#include "sets.h"
#include "scanner.h"
#include "scannerDriver.h"
#include <iomanip>
#include <locale>
#include <string>
#include <stack>
using namespace std;

extern std::stack<string> myStack;
extern Grammar g;
extern std::vector<TermSet> firstSet;
extern std::vector<TermSet> followSet;
extern MarkedVocabulary myVocab;
extern std::vector<std::vector<string> > tableVect;

extern int tokenCode;
extern string tokenText;

int LLDriver()
{

	cout << setw(30) << left <<"PARSE ACTION" << setw(80) << "REMAINING INPUT" << "PARSE STACK" << endl;
	std::vector<string> remainingInput;
	do
	{
		remainingInput.push_back(scanner()); //this doesn't do anything yet so on 26, there is nothing at zero (segfault 11)***********++++++++++*****
	}while(remainingInput[remainingInput.size()-1] != "$");


	cout << endl;













/*
	string currentThing;
	//currentThing = remainingInput[0]; //need to implement scanner to read program. use other one?


	if(currentThing == "-1")
	{
		return -1;
	}
	myStack.push(g.startSymbol);

	while(myStack.size() != 0)
	{

		scannerDriver(tokenCode, tokenText); //these two lines get the next token from scannerDriver
		currentThing = tokenText;

		cout << "current thing is " << currentThing << endl;

		if(existsInVect(myStack.top(), g.nonTerminalsVect))
		{
				if(T(myStack.top(), currentThing) != -1) //T(X, a) doesn't know what to do with the tokens that currentThing is getting from scanner()
				{
					displayLine(T(myStack.top(), currentThing), remainingInput, myStack);
					int index = findIndex(g.LHS, myStack.top());
					std::vector<string> NTProductions;
					NTProductions = getProductions(g.RHS[index]); //this splits up the RHS (y1, y2, ..., yn)
					myStack.pop(); //to get the X out and then replace it with y1, y2...yn below
					for(int i = NTProductions.size()-1; i > -1; --i)	//this puts the split up y's into the stack backwards
					{
						myStack.push(NTProductions[i]);
						displayLine(-1, remainingInput, myStack);
						elimFirst(remainingInput);
					}
				}
				else
				{
					cout << "syntax error on " << currentThing << endl;



					currentThing = scanner(); //moving to next thing to infinite loop is not obtained while repeatedly getting syntax error
					cout << "current thing is " << currentThing << endl;
					//need to advance currentThing I think. Need to do something.
					return -1;
				}
		}
		else //means X is in terminals
		{
			if(myStack.top() == currentThing)
			{
				myStack.pop();
				currentThing = scanner(); //getting next token form user program
				cout << "currentThing is " << currentThing << endl;
				elimFirst(remainingInput);
			}
			else
			{
				cout << "syntax error on " << currentThing << endl;
				//need way to advance here, just like above
			}
		}
	}

 */
	return 0;
}

void elimFirst(std::vector<string>& vect)
{
	vect.erase(vect.begin());//should eliminate first thing in the vector
}

void display(std::vector<string>& vect)
{
	for(int i = 0; i < vect.size(); ++i)
	{
		cout << vect[i];
		if(vect[i] == ";")
		{
			cout << " ";
		}
	}
}

void displayStack(std::stack<string> myStack)
{
	for(int i = 0; i < myStack.size(); ++i)
	{
		cout << myStack.top();
		myStack.pop();
	}
}

void displayLine(int parseAction, std::vector<string> remInput, std::stack<string> myStack) {
	if (parseAction == -1) {
		cout << setw(30) << left << "match";
	}
	else
	{
		cout << setw(30) << left << parseAction;
	}
	//cout << setw(30);
	display(remInput);
	cout << setw(70) << right;
	displayStack(myStack);
	cout << endl;
}