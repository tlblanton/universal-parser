
#include "LLDriver.h"
#include "grammarAnalyzer.h"
#include "sets.h"
#include "scanner.h"
#include "scannerDriver.h"
#include <iomanip>
#include <sstream>
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
	string currentThing;

	int myState = 0;

	cout << setw(30) << left <<"PARSE ACTION" << setw(80) << "REMAINING INPUT" << "PARSE STACK" << endl;
	std::vector<string> remainingInput;
	do
	{
		remainingInput.push_back(scanner()); //this doesn't do anything yet so on 26, there is nothing at zero (segfault 11)***********++++++++++*****
	}while(remainingInput[remainingInput.size()-1] != "$");


	cout << endl;

	myStack.push(g.startSymbol);

	scannerDriver(tokenCode, tokenText);
	currentThing = tokenText;

	while(myStack.size() != 0)
	{
		if(myStack.top() == "(<expression>)")
		{
			myStack.pop();
			myStack.push("<expression>");
		}
		if (existsIn(myStack.top(), g.nonTerminalsVect))
		{
			if (T(myStack.top(), currentThing) != -1)
			{

				myState = T(myStack.top(), currentThing);
				cout << "predict # is " << myState << endl;

				int index = myState - 1;


				myStack.pop();
				std::vector<string> RHSBroken;
				RHSBroken = getRealProductions(g.RHS[index]);
				for(int i = (int)RHSBroken.size()-1; i >= 0; --i)
				{
					myStack.push(RHSBroken[i]);
				}

			}
			else
			{
				cout << "syntax error on nonTerminal \n Trying to find " << currentThing << " in " << myStack.top() << endl;


				scannerDriver(tokenCode, tokenText);
				currentThing = tokenText;
			}
		}
		else		//meaning X is in terminals
		{
			if(myStack.top() == ";" || myStack.top() == "λ")
			{
				myStack.pop();
				cout << "match\n";
				continue;
			}
			if(myStack.top() == "(<expression>)")
			{
				if(myState == 18)
				{
					myStack.pop();
					myStack.push("Id");
				}
				else if(myState == 19)
				{
					myStack.pop();
					myStack.push("IntLiteral");
				}
			}
			else if(myStack.top() == "<primary>")
			{
				if(myState == 18)
				{
					myStack.pop();
					myStack.push("Id");
				}
				else if(myState == 19)
				{
					myStack.pop();
					myStack.push("IntLiteral");
				}
			}


			if(myStack.top() == currentThing || (currentThing == ";" && myStack.top() == "λ"))
			{
				myStack.pop();
				scannerDriver(tokenCode, tokenText);
				currentThing = tokenText;
				cout << "match\n";
				elimFirst(remainingInput);
				cout << "\t\t\t";
				display(remainingInput);
				cout << endl;
			}
			else
			{
				//cout << "mystack.top() is " << myStack.top() << endl;
				//cout << "current thing is " << currentThing << endl;
				//cout << "Syntax error on" << currentThing << endl;
				cout << "\n\n in terminal syntax error \n\n";
			}
		}
	}
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

std::vector<string> getRealProductions(string rhs)
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
			cout << "temp is " << temp << endl;
			/*
			temp = "(";
			rhsBrokenUp.push_back("(");
			string userT;
			getline(ss, userT, ')');
			int count = 1;
			while(temp[count] != ')')
			{
				userT += temp[count];
				count++;
			}
			cout << "userT is " << userT << endl;
			rhsBrokenUp.push_back(userT);

			//rhsBrokenUp.push_back(temp);
			temp = ")";
			 */

		}
		rhsBrokenUp.push_back(temp);
	}

	return rhsBrokenUp;
}