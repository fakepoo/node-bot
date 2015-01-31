#ifndef SOCKET_H
#define SOCKET_H

#include <pthread.h>
#include <vector>
#include "IDataListener.h"

using namespace std;

class Socket
{
    enum SocketStatus // Represents the current status of the socket/connection
    {
        Ready,
        Listening,
        Connected
    };

    enum SocketMode // Represents modes that can be set for the socket
    {
        Idle,  // If connected, the socket will disconnect and await further instructions
        Listen // If not connected, the socket will begin listening for connections
    };

    public:
        Socket(int portNumber);
        virtual ~Socket();

        void start();
        void stop();
        void loop();
        void addListener(IDataListener *dataListener);

    protected:

        int portNumber;
        SocketStatus status;
        SocketMode mode;
        pthread_t loopingThread;
        vector<IDataListener*> dataListeners;

    private:
};

#endif // SOCKET_H
