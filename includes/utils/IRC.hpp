#ifndef IRC_HPP
#define IRC_HPP

#define INITIAL_EVENT_VECTOR_SIZE 16
#define INITIAL_CLIENT_VECTOR_SIZE 8
#define CHANNEL_USER_LIMIT 128

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
#include <signal.h>

#endif
