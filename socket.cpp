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

void Socket::send_to(const sockaddr_in& address, std::atomic_bool& quit,
                     std::vector<std::pair<std::string, int>>* clients_list)
{
    std::string line;
    Message message;
    int result = 0;

    while (!std::cin.eof())
    {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "/quit")
        {
            quit = true;
            break;
        }

        // Clean memory for the message
        memset(message.text, '\0', sizeof(message.text));
        line.copy(message.text, sizeof(message.text) - 1, 0);

        // Send message through socket to all the clients
        if (clients_list != NULL)
        {
            sockaddr_in aux_address;
            int i = 0;
            while ((i < clients_list->size()) && (result >= 0))
            {
                // Build address with ip address and port
                aux_address = Socket::make_ip_address((*clients_list)[i].first,
                                                      (*clients_list)[i].second);
                result = sendto(fd_, &message, sizeof(message), 0,
                    (const sockaddr*) &aux_address, sizeof(aux_address));
                i++;
            }
        }
        else
        {
            result = sendto(fd_, &message, sizeof(message), 0,
                (const sockaddr*) &address, sizeof(address));
        }

        if (result < 0)
        {
            throw std::system_error(errno, std::system_category(),
                "call to sento() function failed");
        }
    }
}

void Socket::receive_from(sockaddr_in& address, std::atomic_bool& quit,
                          std::vector<std::pair<std::string, int>>* clients_list)
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

        // If we are the server and we have receive a message
        // from an unknown client; save it
        if (clients_list != NULL)
        {
            // If that client is not added yet
            if (!std::any_of(clients_list->begin(), clients_list->end(),
                [&remote_ip, &remote_port] (const std::pair<std::string, int>& p)
                    { return p.first == remote_ip && p.second == remote_port; }))
            {
                std::string remote_ip_str(remote_ip);
                clients_list->push_back(std::make_pair(remote_ip_str, remote_port));
            }
        }

        // Print message
        std::cout << " [" << remote_ip << ":" << remote_port << "] says: "
            << message.text << "\n";
    }
}

sockaddr_in Socket::make_ip_address(const std::string ip_address, int port)
{
    // Socket's local address
    sockaddr_in local_address;

    // AF_INET = IPv4
    local_address.sin_family = AF_INET;

    // Listen to all the connections if the specified ip_address
    // is empty. Otherwise, listen to ip_address.
    if (ip_address.empty())
    {
        local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        inet_aton(ip_address.c_str(), &local_address.sin_addr);
    }

    local_address.sin_port = htons(port);
    return local_address;
}
