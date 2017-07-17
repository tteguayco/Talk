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
    //close(fd_);
}

void Socket::send_to(const sockaddr_in& address, std::atomic_bool& quit)
{
    std::string line;
    Message message;

    while (!std::cin.eof())
    {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "/quit")
        {
            quit = true;
            break;
        }

        line.copy(message.text, sizeof(message.text) - 1, 0);

        // Send message through socket
        int result = sendto(fd_, &message, sizeof(message), 0,
            (const sockaddr*) &address, sizeof(address));

        if (result < 0)
        {
            throw std::system_error(errno, std::system_category(),
                "call to sento() function failed");
        }
    }
}

void Socket::receive_from(sockaddr_in& address,std::atomic_bool& quit)
{
    Message message;
    socklen_t src_len = sizeof(address);
    int result, remote_port;
    char* remote_ip;

    while (!quit)
    {
        result = recvfrom(fd_, &message, sizeof(message), 0,
            (sockaddr*) &address, &src_len);

        if (result < 0)
        {
            throw std::system_error(errno, std::system_category(),
                "call to recvfrom() function failed");
        }

        remote_ip = inet_ntoa(address.sin_addr);
        remote_port = ntohs(address.sin_port);

        // Print message
        std::cout << " [" << remote_ip << ":" << remote_port << "] says: "
            << message.text << "\n";
    }
}
