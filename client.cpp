#include "client.h"

Client::Client(const std::string& server_ip_address, int server_port):
    socket_(NULL),
    server_ip_address_(server_ip_address),
    server_port_(server_port)
{}

Client::~Client()
{

}

void Client::run()
{
    sockaddr_in local_address = make_ip_address("127.0.0.1", 8001);
    sockaddr_in remote_address = make_ip_address(server_ip_address_,
                                        server_port_);
    socket_ = new Socket(local_address);

    std::atomic_bool quit(false);

    // Run threads to send and receive messages
    std::thread send_thread(&Socket::send_to, socket_,
        std::ref(remote_address), std::ref(quit));
    std::thread receive_thread(&Socket::receive_from, socket_,
        std::ref(remote_address), std::ref(quit));

    // Wait for the threads to finish safely
    send_thread.join();
    receive_thread.join();

    request_cancellation(send_thread);
    request_cancellation(receive_thread);
}

sockaddr_in Client::make_ip_address(const std::string &ip_address, int port)
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

void Client::request_cancellation(std::thread &thread)
{
    int result = pthread_cancel(thread.native_handle());

    if (result != 0)
    {
        throw std::system_error(result, std::system_category(),
            "cancellation request for a thread failed");
    }
}
