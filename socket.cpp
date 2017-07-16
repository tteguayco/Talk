#include "socket.h"

Socket::Socket(const sockaddr_in &address)
{
    // Create socket
    fd_ = socket(domain, type, protocol);

    if (fd_ < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "unable to create socket");
    }

    // Assign socket to address
    int result = bind(fd_, (const sockaddr*) &address, sizeof(address));

    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "unable to bind socket to address");
    }
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
        throw std::system_error(errno, std::system_category(),
            "call to sento() function failed");
    }
}

void Socket::receive_from(Message &message, sockaddr_in &address)
{
    socklen_t src_len = sizeof(address);

    int result = recvfrom(fd_, &message, sizeof(message), 0,
        (sockaddr*) &address, &src_len);

    if (result < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "call to recvfrom() function failed");
    }

    // Print message
    char* remote_ip = inet_ntoa(address.sin_addr);
    int remote_port = ntohs(address.sin_port);

    std::cout << "[" << remote_ip << ":" << remote_port << "] says: "
        << message.text << "\n";
}
