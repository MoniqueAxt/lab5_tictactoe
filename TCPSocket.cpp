/**
    Lab 5
    Implementation of the TCPSocket class
    @file TCPSocket.cpp
    @author Monique Axt
    @version 1.0
*/

#include "TCPSocket.h"

/*!
 * Constructor that creates a communication endpoint with the returned file
 * descriptor placed in a member variable.
 * Runtime exception thrown if a error occurs during socket creation.
 */
TCPSocket::TCPSocket() {
    iSockDesc = 0;

    if ((iSockDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("\n Socket creation error \n");
        throw std::runtime_error("Socket creation error");
    }
}

/*!
 * Calls a wrapper function to close the socket assigned during construction if it
 * has not already been closed.
 */
TCPSocket::~TCPSocket() {
    close();
}

/*!
 * Connects to the server.
 * Throws runtime errors if an error occurs during the connection process.
 *
 * @param ipaddress address used to connect to the server
 * @param port port used to connect to the server
 */
void TCPSocket::connect(const char *ipaddress, in_port_t port) {
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    /* Convert address to binary form */
    struct addrinfo hints{}; /* in: hints */
    struct addrinfo *res; /* out: result */

    memset(&hints, 0, sizeof(addrinfo)); // zero out the struct

    // Set relevant members
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ipaddress, NULL, &hints, &res) != 0)
        throw std::runtime_error("Invalid address/ Address not supported");

    auto *saddrp = (sockaddr_in *) (res->ai_addr);
    /* Copy binary address */
    memcpy(&serverAddr.sin_addr.s_addr, &saddrp->sin_addr.s_addr, 4);
    freeaddrinfo(res); // release memory allocated by getaddrinfo

    if ((::connect(iSockDesc, (sockaddr *) &serverAddr, sizeof(serverAddr))) == -1) {
        std::perror("Failed to connect");
        throw std::runtime_error("Failed to connect");
    }
}

/*!
 * Overloaded function to connect to the server.
 * Throws runtime errors if an error occurs during the connection process.
 *
 * @param ipaddress hostname address used to connect to the server
 * @param port port used to connect to the server
 */
void TCPSocket::connect(const std::string &ipaddress, in_port_t port) {
    connect(ipaddress.c_str(), port);
}

/*!
 * Wrapper function to close the socket if not already closed.
 * Throws runtime error if socket could not be closed.
 */
void TCPSocket::close() const {
    if (iSockDesc > 0)
        if (r_close(iSockDesc) == -1) {
            throw std::runtime_error("Socket wasn't closed");
        }
    std::cout << "Disconnected." << std::endl;
}




