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
    bool is_server_;

    /**
     * @brief clients_list_
     *
     * Pairs ip/port of those clients which are connected
     * to the server.
     */
    std::vector<std::pair<std::string, int>> clients_list_;

public:
    Client(const std::string& server_ip_address, int server_port, bool is_server);
    ~Client();
    void run();

private:
    void request_cancellation(std::thread& thread);
};

#endif // CLIENT_H
