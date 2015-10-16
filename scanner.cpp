
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
    string file = "tableProg.txt";

    long long pos = 0; //long long because tellg() returns long long
    bool goBack = false;
    if(optional == 1) //pass 1 to just see next token
    {
        goBack = true;
        pos = inFile.tellg();
    }
    else if(optional == 2) //pass 2 to reset
    {
        inFile.close();
        inFile.open(file);
        return "__RESET__";
    }


    if(!inFile.is_open())
    {
        inFile.open(file); //opens file if it is not open already. file must be reachable by program (in same directory ideally)
        if(!inFile)
        {
            cerr << "Cannot open " << file << endl;
            return "-1";
        }
    }

    clearBuffer(tokenBuffer);
    char currentChar;
    if(inspect(inFile) == EOF)
    {
        if(goBack)
        {
            inFile.seekg(pos);
        }
        return "end";
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

                    string temp = checkReserved(tokenBuffer); //function that takes identifiers and converts them to tokens
                    if(temp == "Id")
                    {
                        return tokenBuffer;
                    }
                    else
                    {
                        return temp;
                    }
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
                return tokenBuffer;

            }
            else if(currentChar == '(')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "(";
            }
            else if(currentChar == ')')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return ")";
            }
            else if(currentChar == ';')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return ";";
            }
            else if(currentChar == ',')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return ",";
            }
            else if(currentChar == '+')
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                return "+";
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
                    return ":=";
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
                    return "-";
                }
            }
            else
            {
                if(goBack)
                {
                    inFile.seekg(pos);
                }
                if(currentChar == '$')
                {
                    return "$";
                }
                return "LexicalError(CurrentChar)"; //returning LexicalError here, as the parser will handle throwing the actual error
            }
        }

        if(goBack)
        {
            inFile.seekg(pos);
        }
        return "end";
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
        return "begin";
    }
    else if(id == "END" || id == "end" || id == "End")
    {
        return "end";
    }
    else if(id == "READ" || id == "read")
    {
        return "read";
    }
    else if(id == "WRITE" || id == "write")
    {
        return "write";
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

