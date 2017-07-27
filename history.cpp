#include "history.h"

History::History(const std::string& username)
{
    std::string home_dir = getenv("HOME");
    std::string pathname = home_dir + "/.talk/" + username + ".log";
    int result;
    int mode = 0600;

    // Opens the file for writting,
    // creating the file if it does not already exist.
    // If the file does exist, the system truncates the
    // file to zero bytes.
    fd_ = open(pathname.c_str(), O_RDWR | O_CREAT, mode);

    if (fd_ < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "unable to open file for writting chat history");
    }

    if ((result = ftruncate(fd_, FILE_LEN)) < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "unable to truncate file");
    }

    mapping_address_ = (char*) mmap(NULL, FILE_LEN,
                         PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd_, 0);

    if (mapping_address_ == MAP_FAILED)
    {
        throw std::system_error(errno, std::system_category(),
            "unable to map file on memory");
    }

    // Lock the access to the file
    if (lockf(fd_, F_TLOCK, 0) < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "The specified username is already in use");
    }
}

History::~History()
{
    // Write dummy string at the end of the history
    std::string dummy = END_OF_HISTORY_STRING;
    dummy.copy(mapping_address_, dummy.length(), 0);

    close(fd_);

    if ((munmap(mapping_address_, FILE_LEN)) < 0)
    {
        throw std::system_error(errno, std::system_category(),
            "unable to open file for writting chat history");
    }

    lockf(fd_, F_ULOCK, 0);
}

void History::add_message(const Message& message)
{
    std::string line = std::string(message.sender_username) + ": " +
            std::string(message.text) + '\n';

    int num_bytes = line.copy(mapping_address_, line.length(), 0);
    mapping_address_ += num_bytes;
}
