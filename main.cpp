#include <iostream>
#include <cstring>
#include "socket.h"

sockaddr_in make_ip_address(const std::string& ip_address, int port)
{
    // Socket's local address
    sockaddr_in local_address;

    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(port);

    return local_address;
}

int main()
{
    std::cout << "Program running\n";
    return 0;
}
