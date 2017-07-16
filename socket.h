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

#define MESSAGE_SIZE 1024

const std::string MESSAGE_DEFAULT_TEXT = "Hi!";

struct Message
{
    char text[MESSAGE_SIZE];
    char sender_name[80];
    char send_date[80];
};

class Socket
{
private:
    int fd_;

public:
    Socket(const sockaddr_in& address);
    ~Socket();
    void send_to(const Message& message, const sockaddr_in& address);
    void receive_from(Message& message, sockaddr_in& address);
};

#endif // SOCKET_H
