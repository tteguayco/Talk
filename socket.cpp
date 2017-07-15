#include "socket.h"

Socket::Socket(const sockaddr_in &address)
{
    // Create socket
    fd_ = socket(domain, type, protocol);

    // Assign socket to address
    bind(fd_, (const sockaddr*) &address, sizeof(address));
}

Socket::~Socket()
{
    close(fd_);
}

bool Socket::socket_valid()
{
    if (fd_ >= 0)
    {
        return true;
    }

    return false;
}
