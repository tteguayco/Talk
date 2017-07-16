#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <exception>
#include <atomic>

#define domain AF_INET
#define type SOCK_DGRAM
#define protocol 0

#define SOCKET_CREATION_FAILED 2
#define BIND_FAILED 3

#define MESSAGE_SIZE 1024

struct Message
{
    char text[MESSAGE_SIZE];
};

class Socket
{
private:
    int fd_;

public:
    Socket(const sockaddr_in& address);
    ~Socket();
    void send_to(const sockaddr_in& address, std::atomic_bool& quit);
    void receive_from(sockaddr_in& address, std::atomic_bool& quit);
};

#endif // SOCKET_H
