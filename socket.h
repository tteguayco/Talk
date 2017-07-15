#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{
private:
    int fd_;

public:
    Socket(const sockaddr_in& address);
    ~Socket();
};

#endif // SOCKET_H
