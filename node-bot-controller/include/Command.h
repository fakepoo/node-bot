#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using namespace std;

class Command
{
    public:
        Command(string name, vector<string> parameters);
        virtual ~Command();

        string name;
        vector<string> parameters;

        string toString();
    protected:
    private:
};

#endif // COMMAND_H
