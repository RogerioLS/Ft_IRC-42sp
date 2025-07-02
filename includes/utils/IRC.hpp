#ifndef IRC_HPP
#define IRC_HPP

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

#define INITIAL_EVENT_VECTOR_SIZE 16
#define INITIAL_CLIENT_VECTOR_SIZE 8

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <vector>
#include <cstdlib>
#include <fcntl.h>
#include <sstream>

#endif
