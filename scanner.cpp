
#include "scanner.h"
#include <iostream>
#include "sets.h"
#include "grammarAnalyzer.h"
#include "LLDriver.h"
#include <fstream>

using namespace std;


string tokenBuffer;

/*********************
 SCANNER
 ********************/
string scanner(int optional)//returns token every time it runs. when non-zero argument is passed, it returns the next token but does
//not advance the stream
{

    static ifstream inFile; //static so it retains its open state throughout function calls until EofSym is returned.

    long long pos = 0; //long long because tellg() returns long long
    bool goBack = false;
    if(optional != 0)
    {
        goBack = true;
        pos = inFile.tellg();
    }


    if(!inFile.is_open())
    {
        inFile.open("testProg.txt"); //opens file if it is not open already. file must be reachable by program (in same directory ideally)
    }

    clearBuffer(tokenBuffer);
    char currentChar;
    if(inspect(inFile) == EOF)
    {
        if(goBack)
        {
            inFile.seekg(pos);
        }
        return "EofSym";
    }
    else
    {
        while(inspect(inFile) != EOF)
        {
            currentChar = read(inFile); //read current char
            if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
            {
                advance(inFile);
            }
            else if((currentChar >= 'A' && currentChar <= 'Z') || (currentChar  >= 'a' && currentChar <= 'z'))
            {
                bufferChar(currentChar, tokenBuffer); //put character into tokenBuffer
                while((inspect(inFile) >= 'A' && inspect(inFile) <= 'Z') || (inspect(inFile) >='a' && inspect(inFile) <= 'z') || inspect(inFile) == '_' || (inspect(inFile) >= '0' && inspect(inFile) <= '9'))//checking the next char
                {
                    currentChar = read(inFile);
                    bufferChar(currentChar, tokenBuffer);
                }
                if (!(inspect(inFile) >= 'A' && inspect(inFile) <= 'Z') && !(inspect(inFile) >= 'a' && inspect(inFile) <= 'z') && !(inspect(inFile) >= '0' && inspect(inFile) <= '9'))
                {
                    if(goBack)
                    {
                        inFile.seekg(pos);
                    }
                    return checkReserved(tokenBuffer); //function that takes identifiers and converts them to tokens
                }
            }
            else if(currentChar >= '0' && currentChar <= '9')
            {
                bufferChar(currentChar, tokenBuffer);
                while(inspect(inFile) >= '0' && inspect(inFile) <= '9')
                {
                    currentChar = read(inFile);
                    bufferChar(currentChar, tokenBuffer);
                }
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "IntLiteral";

            }
            else if(currentChar == '(')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "LParen";
            }
            else if(currentChar == ')')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "RParen";
            }
            else if(currentChar == ';')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "SemiColon";
            }
            else if(currentChar == ',')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "Comma";
            }
            else if(currentChar == '+')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "PlusOp";
            }
            else if(currentChar == ':')
            {
                if(inspect(inFile) == '=')
                {
                    currentChar = read(inFile);
                    if(goBack)
                    {
                        inFile.seekg(pos);
                    }
                    return "AssignOp";
                }
                else
                {
                    if(goBack)
                    {
                        inFile.seekg(pos);
                    }
                    return "LexicalError(Inspect)"; //returns LexicalError Token if ':' is not followed by '='
                }
            }
            else if(currentChar == '-')
            {
                if(inspect(inFile) == '-')//comments --
                {
                    currentChar = read(inFile);
                    while(inspect(inFile) != '\n')
                    {
                        read(inFile);
                    }
                }
                else
                {
                    if(goBack)
                    {
                        inFile.seekg(pos);
                    }
                    return "MinusOp";
                }
            }
            else
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "LexicalError(CurrentChar)"; //returning LexicalError here, as the parser will handle throwing the actual error
            }
        }

        if(goBack)
        {
            inFile.seekg(pos);
        }
        return "EofSym";
    }

}

void clearBuffer(string& tokenBuffer)
{
    tokenBuffer = "";
}

char inspect(ifstream& inFile)
{
    return inFile.peek();
}

void bufferChar(char currentChar, string& singleTokenBuffer)
{
    singleTokenBuffer += currentChar;
}


string checkReserved(string id)
{

    if(id == "BEGIN" || id == "begin")
    {
        return "BeginSym";
    }
    else if(id == "END" || id == "end")
    {
        return "EndSym";
    }
    else if(id == "READ" || id == "read")
    {
        return "ReadSym";
    }
    else if(id == "WRITE" || id == "write")
    {
        return "WriteSym";
    }
    else
    {
        return "Id";
    }

}

void advance(ifstream& inFile)
{
    inFile.ignore(0);
}

char read(ifstream& inFile)
{
    return inFile.get();
}

