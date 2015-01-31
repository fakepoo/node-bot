#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include <queue>
#include "IDataListener.h"
#include "Command.h"

using namespace std;

class CommandListener : public IDataListener
{
    public:
        CommandListener(int portNumber);
        virtual ~CommandListener();

        void onDataReceived(char *data, int numberOfBytes);

        Command* getCommand();
        void popCommand();
    private:
        int portNumber;
        queue<char> currentCommandBuffer;
        queue<Command*> commandQueue;

        void clearCommandQueue();
        void clearCurrentCommandBuffer();
        void processCommandBuffer();
};

#endif // COMMANDLISTENER_H
