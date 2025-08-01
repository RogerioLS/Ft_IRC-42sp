/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/08/01 11:30:49 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../utils/IRC.hpp"

class Client;
class Channel;
class Debug;
class CommandHandler;

class Server {

  private:
    int                     _port;
    std::string             _password;
    int                     _serverFd;
    int                     _epollFd;
    std::string             _serverName;
    volatile std::sig_atomic_t _gSignalStatus;
    std::vector<struct epoll_event> _eventsVector;
    int                     _idCounter;
    std::vector<Client>     _clientsVector;
    std::vector<Channel>    _channelsVector;
    Debug&                  _debug;
    CommandHandler*         _commandHandler;

    // Shutdown
    void handleSignal();
    static void handleSigInt(int signum);
    void closeFds();
    // Setup
    void setupServerSocket();
    void setupEpollEvent();
    void setupEpollLoop();
    void setupClientVector();
    // Handle Requests
    void handleNewClient();
    void handleClientRequest(int fd);

  public:
    static Server*    instance;

    Server(char **argv, Debug& debug);
    ~Server();

    void setupServer();

    // Getters
    int getPort() const;
    const std::string& getPassword() const;
    int getServerFd() const;
    int getEpollFd() const;
    int getClientCount() const;
    int getServerRunning() const;
    int getServerIdCounter() const;
    Debug& getDebug();
    Channel* getChannelByName(const std::string& name);
    std::vector<Channel>& getChannelsVector();
    std::vector<Client>& getClientsVector();
    const std::set<int>& getChannelClients(const std::string & channelName) const;
    const std::set<int>& getChannelOpers(const std::string & channelName) const;
    int getClientIdFromNickname(const std::string & clientNickName) const;
    const Client * getClientInstFromId(int clientId) const;
    const std::string& getServerName() const;
    const Client * getClientInstFromNick(const std::string & nickName) const;
    Channel * getChannelInstFromName(const std::string &channelName) const;

    // Setters
    void setServerFd(int serverFd);
    void setEpollFd(int epollFd);
    void setServerRunning(int gSignalStatus);
    void setServerIdCounter(int idCounter);
    // Helpers

    template<typename T>
    void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

    bool isClientFullyRegistered(const std::string & clientNickName) const;
    bool isChannelRegistered(const std::string & channelName) const;
    bool isClientOnChannel(const std::string & clientNickName, std::string & channelName) const;
    bool isClientOperOnChannel(const std::string & clientNickName, std::string & channelName) const;
    std::vector<Client>::iterator clientItFromFd(int fd);
    void createChannel(const std::string& name, Client& client);
    void sendMessage(int fd, const std::string& message);
};

#endif
