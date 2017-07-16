#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <pthread.h>
#include <atomic>
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

void request_cancellation(std::thread& thread)
{
    int result = pthread_cancel(thread.native_handle());

    if (result != 0)
    {
        throw std::system_error(result, std::system_category(),
            "cancellation request for a thread failed");
    }
}

int main(int argc, char *argv[])
{
    std::cout << "** TALK **\n";
    std::cout << "Your credentials:\n";
    std::cout << "\n";

    try
    {
        sockaddr_in local_address = make_ip_address("127.0.0.1", 8001);
        sockaddr_in remote_address = make_ip_address("127.0.0.1", 8000);
        Socket socket(local_address);

        std::atomic_bool quit(false);

        // Run threads to send and receive messages
        std::thread send_thread(&Socket::send_to, socket,
            std::ref(remote_address), std::ref(quit));
        std::thread receive_thread(&Socket::receive_from, socket,
            std::ref(remote_address), std::ref(quit));

        // Wait for the threads to finish safely
        send_thread.join();
        receive_thread.join();
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
