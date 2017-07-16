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

void Socket::receive_from(Message &message, sockaddr_in &address)
{
    socklen_t src_len = sizeof(address);

    int result = recvfrom(fd_, &message, sizeof(message), 0,
        (sockaddr*) &address, &src_len);

    if (result < 0)
    {
        std::cerr << "recvfrom failed: \n";
    }

    // Print message
    char* remote_ip = inet_ntoa(address.sin_addr);
    int remote_port = ntohs(address.sin_port);

    std::cout << "[" << remote_ip << ":" << remote_port << "] says: "
        << message.text << "\n";
}
