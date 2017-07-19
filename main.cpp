#include <getopt.h>

#include "client.h"

#define BAD_ALLOW_ERROR 1
#define SYSTEM_CALL_ERROR 2
#define UNKNOWN_ERROR 3

#define CMDLINE_OPTIONS "hsc:p:"

#define SERVER_PORT 8000

int main(int argc, char *argv[])
{
    int c, option_index;
    std::string remote_server_ip;
    int remote_server_port = 0;
    bool client_mode = false, server_mode = false;
    Client* client = NULL;

    const struct option long_options[] = {
        { "help",   0, 0, 0 },
        { "client", 0, 0, 0 },
        { "server", 0, 0, 0 },
        { "port",   0, 0, 0 }
    };

    std::cout << "TALK";

    // Processing command line
    while ((c = getopt_long(argc, argv, CMDLINE_OPTIONS, long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'h':
                std::cout << "Usage: [...]";
                break;

            case 's':
                server_mode = true;

                break;

            case 'c':
                client_mode = true;
                remote_server_ip = optarg;
                break;

            case 'p':
                remote_server_port = std::stoi(optarg);
                break;
        }
    }

    try
    {
        if (client_mode)
        {
            std::cout << " (client mode)\n\n";
            client = new Client(remote_server_ip, remote_server_port, false);
        }
        else if (server_mode)
        {
            std::cout << " (server mode)\n\n";
            client = new Client(remote_server_ip, remote_server_port, true);
        }

        // Let's go!
        client->run();
    }

    catch (std::bad_alloc& e)
    {
        std::cerr << program_invocation_name << ": insufficient memory\n";
        return BAD_ALLOW_ERROR;
    }

    catch (std::system_error& e)
    {
        std::cerr << program_invocation_name << ": "
            << e.what() << '\n';
        return SYSTEM_CALL_ERROR;
    }

    catch (...)
    {
        std::cerr << program_invocation_name << ": unknown error\n";
        return UNKNOWN_ERROR;
    }

    return 0;
}
