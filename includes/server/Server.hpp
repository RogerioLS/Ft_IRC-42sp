/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/12 16:49:11 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../utils/IRC.hpp"

class Client;

class Channel;

class Server {

  private:
    int                     _port;
    std::string             _password;
    int                     _serverFd;
    int                     _epollFd;
    volatile std::sig_atomic_t _gSignalStatus;
    std::vector<struct epoll_event> _eventsVector;
    int                     _idCounter;
    std::vector<Client>     _clientsVector;
    std::vector<Channel>    _channelsVector;

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

    Server(char **argv);
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
    // Setters
    void setServerFd(int serverFd);
    void setEpollFd(int epollFd);
    void setServerRunning(int gSignalStatus);
    void setServerIdCounter(int idCounter);
    // Helpers
    std::vector<Client>::iterator clientItFromFd(int fd);

    template<typename T>
    void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

    std::vector<Client> &getClientsVector();
    bool isClientRegistered(const std::string & clientNickName) const;
    bool isChannelRegistered(const std::string & channelName) const;
    bool isClientOnChannel(const std::string & clientNickName, std::string & channelName) const;
    bool isClientOperOnChannel(const std::string & clientNickName, std::string & channelName) const;
    std::set<int> getChannelClients(const std::string & channelName) const;
    std::set<int> getChannelOpers(const std::string & channelName) const;
    int getClientIdFromNickname(const std::string & clientNickName) const;
};

#endif
