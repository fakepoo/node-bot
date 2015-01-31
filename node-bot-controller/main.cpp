#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Robot.h"
#include "CommandListener.h"

using namespace std;

void *commandListenerThreadFunction(void *robotPtr);
bool stringToInt(string& s, int* i);

int main()
{
    cout << "Loading NodeBot..." << endl;

    Robot robot;
    CommandListener commandListener(22222);

    // Main loop
    while(true)
    {
        bool sleep = false;

        // Listen  for a command
        cout << "Polling for a command..." << endl;
        Command* pCommand = commandListener.getCommand();
        if(pCommand == NULL)
        {
            cout << "No command found." << endl;
            sleep = true;
        }
        else
        {
            cout << "Command found." << endl;

            // Process the command
            if(pCommand->name == "setVelocity")
            {
                int velocityLeft, velocityRight;
                bool success = pCommand->parameters.size() == 2;
                if(success) success = stringToInt(pCommand->parameters[0], &velocityLeft);
                if(success) success = stringToInt(pCommand->parameters[1], &velocityRight);
                if(success)
                {
                    robot.setVelocity(velocityLeft, velocityRight);
                }
            }

            // Pop the command so we don't process it again
            commandListener.popCommand();
        }

        // TODO: Do we need to process the robot any further outside of a command?

        if(sleep)
        {
            // Sleep the thread for a ms
            int sleepMicroseconds = 1000000;
            usleep(sleepMicroseconds);
        }
    }

    return 0;
}

bool stringToInt(string& s, int* i)
{
    bool success = false;
    stringstream ss(s);
    int x;
    ss >> x;
    if(ss)
    {
        *i = x;
        success = true;
    }
    return success;
}
