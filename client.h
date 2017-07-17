#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <pthread.h>
#include <atomic>

#include "socket.h"

class Client
{
private:
    Socket* socket_;
    std::string server_ip_address_;
    int server_port_;

public:
    Client(const std::string& server_ip_address, int server_port);
    ~Client();
    void run();

private:
    sockaddr_in make_ip_address(const std::string& ip_address, int port);
    void request_cancellation(std::thread& thread);
};

#endif // CLIENT_H
