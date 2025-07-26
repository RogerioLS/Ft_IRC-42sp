#ifndef IRC_HPP
#define IRC_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <vector>
#include <set>
#include <cstdlib>
#include <fcntl.h>
#include <sstream>
#include <csignal>

#define BUFFER_SIZE 4096
#define INITIAL_EVENT_VECTOR_SIZE 10
#define INITIAL_CLIENT_VECTOR_SIZE 10
#define MAX_PASSWORD_LENGTH 20


#endif
