#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define domain AF_INET
#define type SOCK_DGRAM
#define protocol 0

#define SOCKET_CREATION_FAILED 2
#define BIND_FAILED 3

class Socket
{
private:
    int fd_;

public:
    Socket(const sockaddr_in& address);
    ~Socket();
    bool socket_valid();
};

#endif // SOCKET_H
