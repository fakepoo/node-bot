#include "CommandListener.h"
#include <iostream>
#include "Socket.h"

using namespace std;

// All Commands should be in the form of <command_name|parameter_1|parameter_2|...|parameter_X>

CommandListener::CommandListener(int portNumber)
{
    Socket socket(22222);
    socket.addListener(this);
    socket.start();
}

CommandListener::~CommandListener()
{
    clearCommandQueue();
}

void CommandListener::clearCommandQueue()
{
    while(!commandQueue.empty())
    {
        Command *command = commandQueue.front();
        commandQueue.pop();
        delete command;
    }
}

void CommandListener::onDataReceived(char *data, int numberOfBytes)
{
    for(int i = 0; i < numberOfBytes; ++i)
    {
        // Is this the beginning of a command?
        if(data[i] == '<')
        {
            clearCurrentCommandBuffer();
            currentCommandBuffer.push(data[i]);
        }
        // Is this the end of command character?
        else if(data[i] == '>')
        {
            currentCommandBuffer.push(data[i]);
            processCommandBuffer();
        }
        // Otherwise, make sure we have started a command already
        else if(!currentCommandBuffer.empty())
        {
            currentCommandBuffer.push(data[i]);
        }
    }
}

void CommandListener::clearCurrentCommandBuffer()
{
    while(!currentCommandBuffer.empty()) currentCommandBuffer.pop();
}

// It is assumed that the currentCommandBuffer contains something that starts with '<' and ends with '>'.
void CommandListener::processCommandBuffer()
{
    string commandName = "";
    vector<string> parameters;

    // Peel off the first character (the '<')
    currentCommandBuffer.pop();

    // Read the command first
    while(currentCommandBuffer.front() != '>'){
        char c = currentCommandBuffer.front();
        currentCommandBuffer.pop();
        if(c == '|') break;
        commandName.push_back(c);
    }

    // Read parameters
    if(currentCommandBuffer.front() != '>')
    {
        string parameter = "";
        while(currentCommandBuffer.front() != '>')
        {
            char c = currentCommandBuffer.front();
            currentCommandBuffer.pop();
            if(c == '|')
            {
                parameters.push_back(parameter);
                parameter = "";
            }
            parameter.push_back(c);
        }
        parameters.push_back(parameter);
    }

    // Clear the buffer
    clearCurrentCommandBuffer();

    // Add the command to the commands queue (so that it can be read from the main thread)
    Command *command = new Command(commandName, parameters);
    commandQueue.push(command);
    cout << "Command Received: " << command->toString() << endl;
}

Command* CommandListener::getCommand()
{
    Command* command = NULL;
    if(!commandQueue.empty())
    {
        command = commandQueue.front();
    }

    return command;
}

void CommandListener::popCommand()
{
    if(!commandQueue.empty())
    {
        commandQueue.pop();
    }
}
