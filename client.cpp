#include "client.h"

Client::Client(const std::string& server_ip_address, int server_port,
               bool is_server, std::string username):
    socket_(NULL),
    server_ip_address_(server_ip_address),
    server_port_(server_port),
    is_server_(is_server),
    clients_list_(),
    username_(username)
{}

Client::~Client()
{

}

void Client::run()
{
    sockaddr_in local_address;
    sockaddr_in remote_address;
    std::vector<std::pair<std::string, int>>* clients_list = NULL;

    if (is_server_)
    {
        local_address = Socket::make_ip_address("127.0.0.1", server_port_);
        remote_address = Socket::make_ip_address("", 0);
        clients_list = new std::vector<std::pair<std::string, int>>();
    }
    else
    {
        // Setting port to '0' will make the OS assign a free port
        local_address = Socket::make_ip_address("127.0.0.1", 0);
        remote_address = Socket::make_ip_address(server_ip_address_, server_port_);
    }

    socket_ = new Socket(local_address);

    std::atomic_bool quit(false);

    // Run threads to send and receive messages
    std::thread send_thread(&Socket::send_to, socket_,
        std::ref(remote_address), std::ref(quit), clients_list, username_);
    std::thread receive_thread(&Socket::receive_from, socket_,
        std::ref(remote_address), std::ref(quit), clients_list);

    // Wait for the threads to finish safely
    send_thread.join();
    receive_thread.join();

    request_cancellation(send_thread);
    request_cancellation(receive_thread);
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
