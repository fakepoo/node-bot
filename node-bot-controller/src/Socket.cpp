#include "Socket.h"
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>

using namespace std;

void *looping_thread_function(void *ptr);

Socket::Socket(int portNumber)
{
    this->portNumber = portNumber;
    this->status = Ready;
    this->mode = Idle;

    // Start the loop in a separate thread
    int retVal = pthread_create(&this->loopingThread, NULL, looping_thread_function, (void*) this);
    if(retVal)
    {
        cout << "COULD NOT CREATE THE THREAD. RETURN CODE: " << retVal << endl;
    }
}

Socket::~Socket()
{
    stop();
}

void Socket::addListener(IDataListener *dataListener)
{
    dataListeners.push_back(dataListener);
}

void Socket::start()
{
    // Put the socket in listen mode
    this->mode = Listen;

}

void Socket::stop()
{
    // Put the socket in Idle mode
    this->mode = Idle;

    // TODO: wait for the thread to join
    pthread_join(this->loopingThread, NULL);
}

// This works in blocking mode

void Socket::loop()
{
    int rc;
    struct addrinfo hints, *res;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    char portNumberString[32] = "";
    sprintf(portNumberString, "%d", this->portNumber);

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((rc = getaddrinfo(NULL, portNumberString, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        // TODO: Throw an exception?
    }
    // servinfo now points to a linked list of 1 or more struct addrinfos

    // make a socket, bind it, and listen on it:
    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int yes = 1;
    rc = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if(rc != 0)
    {
        // TODO: What error code is this?
    }
    bind(socket_fd, res->ai_addr, res->ai_addrlen);
    listen(socket_fd, 1); // Listen for a maximum of 1 connection

    while(true)
    {
        bool connected = false;

        this->status = Listening;
        cout << "Waiting for a connection..." << endl;

        // accept an incoming connection:
        addr_size = sizeof their_addr;
        int connection_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);

        connected = true;
        this->status = Connected;

        // ready to communicate on socket descriptor new_fd!
        while(connected)
        {
            cout << "Connection established." << endl;

            // Send an initial message
            char *message = "You've connected to the CommandListener.\r\n";
            int numberOfBytesSent = send(connection_fd, message, strlen(message), 0);
            if(numberOfBytesSent < 0)
            {
                cout << "An error has occurred. errno: " << errno << endl;
                connected = false;
            }
            else
            {
                cout << numberOfBytesSent << " bytes sent." << endl;
            }

            // Keep listening
            char buffer[1024];
            while(connected)
            {
                int numberOfBytesRead = recv(connection_fd, buffer, 1024, 0);

                // Was there an error?
                if(numberOfBytesRead < 0)
                {
                    cout << "An error has occurred. errno: " << errno << " - " << strerror(errno) << endl;
                    connected = false;
                    break;
                }

                // Was the connection closed?
                if(numberOfBytesRead == 0)
                {
                    connected = false;
                    break;
                }

                if(numberOfBytesRead > 0)
                {
                    // Print out the bytes
                    for(int i = 0; i < numberOfBytesRead; ++i)
                    {
                        cout << buffer[i];
                    }

                    // Notify the listeners
                    for(int i = 0; i < dataListeners.size(); ++i)
                    {
                        dataListeners[i]->onDataReceived(buffer, numberOfBytesRead);
                    }
                }
            }

            cout << "Connection disconnected." << endl;
        }

        close(connection_fd);
    }
    close(socket_fd);
    freeaddrinfo(res); // free the linked-list

}

/* // TODO: Adjust this code (non-blocking sockets) to take over the implementation.
void Socket::loop()
{
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    char portNumberString[32] = "";
    sprintf(portNumberString, "%d", this->portNumber);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, portNumberString, &hints, &ai)) != 0) {
        cout << "Socket::Loop - ERROR - getaddrinfo: " << gai_strerror(rv) << endl;
        // TODO: Throw an exception or something
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        cout << "Socket::Loop - ERROR - failed to bind." << endl;
        // TODO: Throw an exception or something
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        cout << "Socket::Loop - ERROR - failed to listen." << endl;
        // TODO: Throw an exception or something
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            cout << "Socket::Loop - ERROR - select() failed." << endl;
            // TODO: Throw an exception or something
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }

                        cout << "Connection established." << endl;
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client
                        for(j = 0; j <= fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &master)) {
                                // except the listener and ourselves
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
}
*/

void *looping_thread_function(void *ptr)
{
    Socket *socket = (Socket*) ptr;
    socket->loop();

    return NULL;
}

