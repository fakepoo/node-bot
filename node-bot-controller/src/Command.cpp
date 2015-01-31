#include "Command.h"
#include <iostream>

using namespace std;

Command::Command(string name, vector<string> parameters)
{
    this->name = name;
    this->parameters = parameters;
}

Command::~Command()
{
}

string Command::toString()
{
    string s = name + "(";
    for(int i = 0; i < parameters.size(); ++i)
    {
        if(i > 0) s += ", ";
        s += parameters[i];
    }
    s += ")";
    return s;
}
