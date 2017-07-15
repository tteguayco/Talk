#include <iostream>
#include <cstring>
#include "socket.h"

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
    std::cout << "Program running\n";
    return 0;
}
