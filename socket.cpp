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

void Socket::send_to(const Message &message, const sockaddr_in &address)
{
    // Send message through socket
    int result = sendto(fd_, &message, sizeof(message), 0,
        (const sockaddr*) &address, sizeof(address));

    if (result < 0)
    {
        std::cerr << "sento() failed: \n";
    }
}
