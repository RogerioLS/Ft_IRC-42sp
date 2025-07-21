#ifndef ISERVER_HPP
#define ISERVER_HPP

#include <string>

// Forward declarations to avoid circular dependencies
class Client;
class Channel;

class IServer {
public:
    virtual ~IServer() {}

    virtual Channel* getChannelByName(const std::string& name) = 0;
    virtual Client* getClientByNickname(const std::string& nickname) = 0;
    virtual Client* getClientById(int id) = 0;
    virtual void createChannel(const std::string& name, Client& client) = 0;
    virtual const std::string& getServerName() const = 0;
    virtual void sendMessage(int fd, const std::string& message) = 0;
};

#endif
