#include <getopt.h>

#include "client.h"

#define BAD_ALLOW_ERROR 1
#define SYSTEM_CALL_ERROR 2
#define UNKNOWN_ERROR 3

#define CMDLINE_OPTIONS "hsc:p:"
#define SERVER_PORT 8000

const std::string HELP_LINE = "\nTALK. A simple chat written in C++ based on Unix sockets.\n\n"
    "Usage:\n"
    "  talk (-h | --help)\n"
    "  talk (-c | --client) <server_ip> (-p | --port) <server_port>\n"
    "  talk (-s | --server) (-p | --port) <server_port>\n"
    "\n\n"
    "Options:\n"
    "  -h --help      Shows this screen.\n"
    "  -s --server    Starts with the server mode.\n"
    "  -c --client    Starts with the client mode.\n"
    "  -p --port      Sets the server listening port.\n\n";

int main(int argc, char *argv[])
{
    int c, option_index;
    std::string remote_server_ip;
    int remote_server_port = 0;
    bool client_mode = false, server_mode = false;
    Client* client = NULL;
    bool start = false;

    /**
      * More info about the fields of this struct
      * in: https://linux.die.net/man/3/getopt_long
      */
    const struct option long_options[] = {
        { "help",   0, 0, 'h' },
        { "client", 1, 0, 's' },
        { "server", 0, 0, 's' },
        { "port",   1, 0, 'p' }
    };

    // Processing command line
    while ((c = getopt_long(argc, argv, CMDLINE_OPTIONS, long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'h':
                std::cout << HELP_LINE;
                break;

            case 's':
                server_mode = true;
                start = true;
                break;

            case 'c':
                client_mode = true;
                remote_server_ip = optarg;
                start = true;
                break;

            case 'p':
                remote_server_port = std::stoi(optarg);
                start = true;
                break;
        }
    }

    try
    {
        if (start)
        {
            std::cout << "TALK";
            if (client_mode)
            {
                std::cout << "\n\n> ";
                client = new Client(remote_server_ip, remote_server_port, false);
            }
            else if (server_mode)
            {
                std::cout << " (server mode)\n\n> ";
                client = new Client(remote_server_ip, remote_server_port, true);
            }

            // Let's go!
            if (client != NULL)
            {
                client->run();
            }
        }
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
