/**
    Lab 5
    Definition of the TCPSocket class
    @file TCPSocket.h
    @author Monique Axt
    @version 1.0
*/

#ifndef LAB5_TCP_TCPSOCKET_H
#define LAB5_TCP_TCPSOCKET_H

#include "Restart.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <stdexcept>
#include <netdb.h>

#include <string>

class TCPSocket {
private:
/** socket descriptor*/
    int iSockDesc;

public:
/** Constructor
* Creates the socket, may throw runtime_error. */
    TCPSocket();

/** Destructor
* Closes the socket, if not done before.*/
    ~TCPSocket();

/** Connects to server using a hostname or
* a dotted decimal address. May throw runtime_error*/
    void connect(const char *ipaddress, in_port_t port);

    void connect(const std::string &ipaddress, in_port_t port);

/** Returns the socket descriptor*/
    int getDescriptor() const { return iSockDesc; }

/** Closes the socket. May throw runtime_error .*/
    void close() const;

};


#endif //LAB5_TCP_TCPSOCKET_H
