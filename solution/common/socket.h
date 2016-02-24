#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "help_printer.h"
#include "utility.h"

//#define SOCKET_CHECK_ERRORS

class Socket {
private:
    int handle;
    bool connected;
    char line_buf[1024];
    int line_index;

    static sockaddr_in create_address(in_addr address, unsigned short port);
    static sockaddr_in create_address(unsigned short port);

    void disconnect();

public:
    Socket();
    explicit Socket(int handle);
    ~Socket();

    int get_handle();

    void bind(unsigned short port);
    bool connect(const char* host, unsigned short port);

    bool write(const char* string);
    bool write(const char* buffer, size_t length);

    bool has_data();
    bool has_data(timeval timeout);
    
    bool is_connected();

    int read(char* buffer, size_t max_length);
    int readline(char* buffer, size_t max_length);

    void get_info(char* host, unsigned short* port);
    void get_remote_info(char* host, unsigned short* port);

    void close();
};
