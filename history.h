#ifndef HISTORY_H
#define HISTORY_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MESSAGE_SIZE 1024
#define FILE_LEN 1000000
#define END_OF_HISTORY_STRING "***///***"

struct Message
{
    char text[MESSAGE_SIZE];
    char sender_ip[80];
    int sender_port;
    char sender_username[80];
};

class History
{
private:
    int fd_;
    char* mapping_address_;
    std::string username_;

public:
    History(const std::string& username);
    ~History();
    void add_message(const Message& message);
};

#endif // HISTORY_H
