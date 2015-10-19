//
// Created by Tyler Blanton on 10/15/15.
//

#include "scannerDriver.h"
#include <string>

int tokenCode;
string tokenText;
int state;
static fstream inFile("tableProg.txt");

/************************************ | SCANNER | ********************************************/
void scannerDriver(int &tokenCode, string &tokenText)
{
    state = 0; //the start state
    tokenText = "";
    tokenCode = -1;
    bool end = false;
    while(!end)
    {

        if(currentChar() == EOF)
        {
            tokenText = "EofSym";
            return;
        }
        else if(currentChar() == '$')
        {
            tokenText = '$';
            return;
        }
        string actionResult = action(state, currentChar());
        if(actionResult == "Error")
        {
            cout << "Lexical Error" <<  endl;
        }
        else if(actionResult == "MoveAppend")
        {
            state = nextState(state, currentChar());
            tokenText += currentChar();//append currentChar
            consumeChar();
        }
        else if(actionResult == "MoveNoAppend")
        {
            state = nextState(state, currentChar());
            consumeChar();
        }
        else if (actionResult == "HaltAppend")
        {
            lookupCode(state, currentChar(), tokenCode);
            tokenText += currentChar();
            checkExceptions(tokenCode, tokenText);
            consumeChar();
            if(tokenCode == 0)
            {
                scannerDriver(tokenCode, tokenText);
            }
            end = true;//getting out of scanner
        }
        else if(actionResult == "HaltNoAppend")
        {

            lookupCode(state, currentChar(), tokenCode);
            checkExceptions(tokenCode, tokenText);
            consumeChar();
            if(tokenCode == 0)
            {
                scannerDriver(tokenCode, tokenText);
            }

            end = true;//get out of scanner
        }
        else if(actionResult == "HaltReuse")
        {
            lookupCode(state, currentChar(), tokenCode);
            checkExceptions(tokenCode, tokenText);
            if(tokenCode == 0)
            {
                scannerDriver(tokenCode, tokenText);
            }
            end = true;//get out of scaner
        }
    }
    //end of while
}

/************************************ | FUNCTIONS | ********************************************/
//LOOK UP CODE
void lookupCode(int state, char currentChar, int &tokenCode)//when any of the halts happen, this function is called to check if the
{
    if(tokenText[0] == '-' && tokenText[1] == '-')
    {
        tokenCode = 0;
    }

    else if(tokenText == "begin" || tokenText == "Begin" || tokenText == "BEGIN")
    {
        tokenCode = 21;
    }
    else if(tokenText == "end" || tokenText == "End" || tokenText == "END")
    {
        tokenCode = 22;
    }
    else if(nextState(state, currentChar) == 13)
    {
        tokenCode = 0;
    }
    else if(nextState(state, currentChar) == 19)
    {
        tokenCode = 19;
    }
    else if(nextState(state, currentChar) == 20)
    {
        tokenCode = 20;
    }
    else if(nextState(state, currentChar) == 18)
    {
        tokenCode = 18;
    }
    else if(tokenText == "-")
    {
        tokenCode = 25;
    }
    else if(nextState(state, currentChar) == 14)
    {
        tokenCode = 14;
    }
    else if(nextState(state, currentChar) == 12)
    {
        tokenCode = 12;
    }
    else if(nextState(state, currentChar) == 16)
    {
        tokenCode = 16;
    }
    else if(nextState(state, currentChar) == 11 && (tokenText == "write" || tokenText == "Write" || tokenText == "WRITE"))
    {
        tokenCode = 23;
    }
    else if(nextState(state, currentChar) == 11 && (tokenText == "read" || tokenText == "Read" || tokenText == "READ"))
    {
        tokenCode = 24;
    }
    else if((nextState(state, currentChar) == 11) && tokenText != "begin" && tokenText != "end" && tokenText != "EofSym" && tokenText != "write" )
    {
        tokenCode = 11;
    }
    else if(nextState(state, currentChar) == 17)
    {
        tokenCode = 17;
    }
}

//CHECK EXCEPTIONS
void checkExceptions(int &tokenCode, string &tokenText)
{
    if(tokenCode == 19)
    {
        tokenText = "(";
        //tokenText = "LParen";
    }
    else if(tokenCode == 17)
    {
        tokenText = ",";
        //tokenText = "Comma";
    }
    else if(tokenCode == 20)
    {
        tokenText = ")";
        //tokenText = "RParen";
    }
    else if(tokenCode == 14)
    {
        tokenText = "+";
        //tokenText = "PlusOp";
    }
    else if(tokenCode == 18)
    {
        tokenText = ";";
        //tokenText = "SemiColon";
    }
    else if(tokenCode == 21)
    {
        tokenText = "begin";
        //tokenText = "BeginSym";
    }
    else if(tokenCode == 22)
    {
        tokenText = "end";
        //tokenText = "EndSym";
    }
    else if(tokenCode == 12)
    {
        tokenText = "IntLiteral";
    }
    else if(tokenCode == 16)
    {
        tokenText = ":=";
        //tokenText = "AssignOp";
    }
    else if(tokenCode == 11)
    {
        //cout << "token Text is " << tokenText << endl;
        tokenText = "Id";
    }
    else if(tokenCode == 23)
    {
        tokenText = "write";
        //tokenText = "WriteSym";
    }
    else if(tokenCode == 24)
    {
        tokenText = "read";
        //tokenText = "ReadSym";
    }
    else if(tokenCode == 25)
    {
        tokenText = "-";
        //tokenText = "MinusOp";
    }

}

/************************************ | ACTION | ********************************************/
string action(int state, char currentChar)
{
    if(state == 0)
    {
        if(isalnum(currentChar) || currentChar == '-' || currentChar == ':')
        {
            return "MoveAppend";
        }
        else if(currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
        {
            return "MoveNoAppend";
        }
        else if(currentChar == '+' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')')
        {
            return "HaltAppend";
        }
        else if(currentChar == '=' || currentChar == '_')
        {
            return "Error";
        }
        else
        {
            return "Error";
        }
    }
    else if(state == 1)
    {
        if(isalnum(currentChar) || currentChar == '_')
        {
            return "MoveAppend";
        }
        else if(currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
        {
            return "HaltNoAppend";
        }
        else if(currentChar == '+' || currentChar == '-' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')')
        {
            return "HaltReuse";
        }

    }
    else if(state == 2)
    {
        if(isalpha(currentChar))
        {
            return "HaltNoAppend";
        }
        else if(isnumber(currentChar))
        {
            return "MoveAppend";
        }
        else if(currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '_')
        {
            return "HaltReuse";
        }
        else if(currentChar == '\t' || currentChar == '\n' || currentChar == ' ')
        {
            return "HaltNoAppend";
        }
        else
        {
            return "Error";
        }
    }
    else if(state == 3)
    {
        if(isalnum(currentChar) || currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '_')
        {
            return "HaltReuse";
        }
        else if(currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
        {
            return "MoveNoAppend";
        }
    }
    else if(state == 4)
    {
        if(isalnum(currentChar) || currentChar == ' ' || currentChar == '+' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '\t' || currentChar == '\n')
        {
            return "HaltReuse";
        }
        else if(currentChar == '-')
        {
            return "MoveAppend";
        }
        else if (currentChar == '_')
        {
            return "Error";
        }
        else
        {
            return "Error";
        }
    }
    else if(state == 5)
    {
        if(currentChar == '\n')
        {
            return "HaltNoAppend";
        }
        else
        {
            return "MoveAppend";
        }
    }
    else if(state == 6)
    {
        if(currentChar == '=')
        {
            return "HaltAppend";
        }
        else
        {
            return "Error";
        }
    }


    return "nothing";
}


/************************************ | NEXTSTATE | ********************************************/
int nextState(int state, char currentChar)
{
    if(state == 0)
    {
        if(isalpha(currentChar))
        {
            return 1;
        }
        else if(isnumber(currentChar))
        {
            return 2;
        }
        else if(currentChar == ' ')
        {
            return 3;
        }
        else if(currentChar == '+')
        {
            return 14;
        }
        else if(currentChar == '-')
        {
            return 4;
        }
        else if(currentChar == ':')
        {
            return 6;
        }
        else if(currentChar == ',')
        {
            return 17;
        }
        else if(currentChar == ';')
        {
            return 18;
        }
        else if(currentChar == '(')
        {
            return 19;
        }
        else if(currentChar == ')')
        {
            return 20;
        }
        else if(currentChar == '\t' || currentChar == '\n')
        {
            return 3;
        }
    }
    else if(state == 1)
    {
        if(isalnum(currentChar) || currentChar == '_')
        {
            return 1;
        }
        else if(currentChar == ' ' || currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '\t' || currentChar == '\n')
        {
            return 11;
        }
        else
        {
            cout << "error in state " << state << " finding char " << currentChar << endl;
        }
    }
    else if(state == 2)
    {
        if(isalpha(currentChar) || currentChar == ' ' || currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '_' || currentChar == '\t' || currentChar == '\n')
        {
            return  12;
        }
        else if(isnumber(currentChar))
        {
            return 2;
        }
    }
    else if(state == 3)
    {
        if(isalnum(currentChar) || currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '_')
        {
            return 13;
        }
        else if(currentChar == '\t' || currentChar == '\n' || currentChar == ' ')
        {
            return 3;
        }
    }
    else if(state == 4)
    {
        if(isalnum(currentChar) || currentChar == ' ' || currentChar == '+' || currentChar == '=' || currentChar == ':' || currentChar == ',' || currentChar == ';' || currentChar == '(' || currentChar == ')' || currentChar == '\t' || currentChar == '\n')
        {
            return 21;
        }
        else if(currentChar == '-')
        {
            return 5;
        }
    }
    else if(state == 5)
    {
        if(currentChar == '\n')
        {
            return 15;
        }
        else
        {
            return 5;
        }
    }
    else if(state == 6)
    {
        if(currentChar == '=')
        {
            return 16;
        }
    }
    return 0; //default return value
}

char currentChar()
{
    return inFile.peek();
}

void consumeChar()
{
    inFile.get();
}