#include <iostream>
#include <cstring>
#include <sstream>
#include "socket.h"

#define BAD_ALLOW_ERROR 1
#define SYSTEM_CALL_ERROR 2
#define UNKNOWN_ERROR 3

sockaddr_in make_ip_address(const std::string& ip_address, int port)
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

int main(int argc, char *argv[])
{
    try
    {
        sockaddr_in local_address = make_ip_address("127.0.0.1", 3001);
        sockaddr_in remote_address = make_ip_address("127.0.0.1", 3000);
        Socket local_socket(local_address);

        std::string line;
        Message message;

        while (!std::cin.eof())
        {
            std::cout << "> ";
            std::getline(std::cin, line);

            if (line == "/quit") break;

            line.copy(message.text, sizeof(message.text) - 1, 0);
            local_socket.send_to(message, remote_address);
            local_socket.receive_from(message, remote_address);
        }
    }

    catch (std::bad_alloc& e)
    {
        std::cerr << program_invocation_short_name << ": insufficient memory\n";
        return BAD_ALLOW_ERROR;
    }

    catch (std::system_error& e)
    {
        std::cerr << program_invocation_short_name << ": "
            << e.what() << '\n';
        return SYSTEM_CALL_ERROR;
    }

    catch (...)
    {
        std::cerr << program_invocation_short_name << ": unknown error\n";
        return UNKNOWN_ERROR;
    }

    return 0;
}
