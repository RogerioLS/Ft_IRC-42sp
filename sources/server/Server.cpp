/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:31 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:34:32 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Channel.hpp"
#include "../includes/parser/Parser.hpp"
#include "../includes/command/CommandHandler.hpp"

Server::Server(char **argv, Debug& debug)
 :  _port(std::atoi(argv[1])),
    _password(argv[2]),
    _serverFd(-1),
    _epollFd(-1),
    _serverName("irc.42sp"),
    _gSignalStatus(-1),
    _idCounter(0),
    _debug(debug)
  {
  _commandHandler = new CommandHandler(*this, _debug);
  _debug.debugPrint("✅ Server initialized", GREEN);
  instance = this;
  }

Server::~Server()
{
  delete _commandHandler;
  closeFds();
  _debug.debugPrint("✅ Server destroyed", RED);
}

void Server::setupServer() {
  handleSignal();
  setupServerSocket();
  setupEpollEvent();
  setupClientVector();
  setupEpollLoop();
}

// Shutdown
void Server::handleSignal() {
  signal(SIGINT, handleSigInt);
}

void Server::handleSigInt(int signum) {
  if (instance)
    instance->_gSignalStatus = signum;
}

void Server::closeFds() {

  if (getServerFd() != -1)
    close(getServerFd());

  if (getEpollFd() != -1)
    close(getEpollFd());

  std::vector<Client>::iterator it = _clientsVector.begin();
  for(std::vector<Client>::iterator ite = _clientsVector.end(); it != ite; it++) {
    int clientFd = it->getClientFd();
    if (clientFd != -1)
      close(clientFd);
  }
}

// Setup
void Server::setupServerSocket() {

  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(getPort());

  setServerFd(socket(AF_INET, SOCK_STREAM, 0));
  if (getServerFd() < 0)
    throw std::runtime_error("Error creating socket");

  if (fcntl(getServerFd(), F_SETFL, O_NONBLOCK) < 0)
    throw std::runtime_error("Error setting socket option O_NONBLOCK");

  if (bind(getServerFd(), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    throw std::runtime_error("Error at binding.");

  if (listen(getServerFd(), SOMAXCONN) < 0)
    throw std::runtime_error("Error at listening.");
}

void Server::setupEpollEvent() {

  setEpollFd(epoll_create(1));
  if (getEpollFd() < 0)
    throw std::runtime_error("Error creating epool");

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = getServerFd();

  if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, getServerFd(), &ev) < 0)
    throw std::runtime_error("Error epoll listen server fd");

  _eventsVector.reserve(INITIAL_EVENT_VECTOR_SIZE);
  _eventsVector.push_back(ev);
  setServerRunning(1);
}

void Server::setupClientVector() {
  if (_clientsVector.empty())
    _clientsVector.reserve(INITIAL_CLIENT_VECTOR_SIZE);
}

void Server::setupEpollLoop() {

  while (getServerRunning() == 1) {

    int nfds = epoll_wait(_epollFd, _eventsVector.data(), _eventsVector.size(), 0);
    if (nfds < 0)
      throw std::runtime_error("Error epoll_wait");

    for (int n = 0; n < nfds; ++n) {
      int clientFd = _eventsVector[n].data.fd;
      if (clientFd == getServerFd())
        handleNewClient();
      else
        handleClientRequest(clientFd);
      resizeVector(static_cast<std::size_t>(nfds), _eventsVector);
      resizeVector(_clientsVector.size(), _clientsVector);
    }
  }
  std::cout << RED << "[SIGNAL] SIGINT recebido!" << RESET << std::endl;
}

// Handle Requests
void Server::handleNewClient() {

  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_len = sizeof(client_addr);
  int conn_socket = accept(getServerFd(),(struct sockaddr *) &client_addr, &client_len);
  if (conn_socket == -1)
    std::cerr << YELLOW << "Failed to accept client connection. Retrying..." << RESET << std::endl;
  else {
    if (fcntl(conn_socket, F_SETFL, O_NONBLOCK) < 0) {
      std::cerr << YELLOW << "Failed to set client connection to non block. Retrying..." << RESET << std::endl;
      return;
    }

    epoll_event client_event;
    client_event.events = EPOLLIN | EPOLLET;
    client_event.data.fd = conn_socket;
    std::string clientIp = inet_ntoa(client_addr.sin_addr);
    uint16_t clientPort = ntohs(client_addr.sin_port);

    std::cout << GREEN << "Client connected. " << "fd: " << conn_socket <<  RESET << std::endl;
    _clientsVector.push_back(Client(conn_socket, getServerIdCounter(), clientPort, clientIp));
    setServerIdCounter(getServerIdCounter() + 1);

    if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, conn_socket, &client_event) < 0) {
      _debug.debugPrint("Error epoll listen client fd", RED);
      return;
    }
    _debug.debugPrint("Client count: " + Utils::intToString(getClientCount()), CYAN);
  }
}

void Server::handleClientRequest(int clientFd) {

  std::vector<Client>::iterator it = clientItFromFd(clientFd);
  if (it != _clientsVector.end()) {
    std::cout << MAGENTA << "Client interacted." << " fd: " << clientFd << RESET << std::endl;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
      std::cerr << YELLOW << "Client id: " << it->getClientId() << " disconnected" << RESET << std::endl;
      close(it->getClientFd());
      _clientsVector.erase(it);
    } else {
      buffer[bytesRead] = '\0';
      it->appendClientBuffer(std::string(buffer));
      Parser::appendParsedCommand(*it);

      std::vector<std::string> commands = it->getClientParsedCommand();
      for (size_t i = 0; i < commands.size(); ++i) {
        _commandHandler->executeCommand(*it, commands[i]);
      }
      it->clearParsedCommands();
    }
  }
}

// Getters
int Server::getPort() const { return _port; }

const std::string& Server::getPassword() const { return _password; }
const std::string& Server::getServerName() const { return _serverName; }
int Server::getServerFd() const { return _serverFd; }
int Server::getEpollFd() const { return _epollFd; }
int Server::getClientCount() const { return _clientsVector.size(); }
int Server::getServerRunning() const { return _gSignalStatus; }
int Server::getServerIdCounter() const { return _idCounter; }
Debug& Server::getDebug() { return _debug; }

Channel* Server::getChannelByName(const std::string& name) {
  for (std::vector<Channel>::iterator it = _channelsVector.begin(); it != _channelsVector.end(); ++it) {
    if (it->getName() == name) {
      return &(*it);
    }
  }
  return NULL;
}

std::vector<Channel> Server::getChannelsVector() const { return _channelsVector; }

// Setters
void Server::setServerFd(int serverFd) { _serverFd = serverFd; }
void Server::setEpollFd(int epollFd) { _epollFd = epollFd; }
void Server::setServerRunning(int gSignalStatus) { _gSignalStatus = gSignalStatus; }
void Server::setServerIdCounter(int idCounter) { _idCounter = idCounter; }

// Helpers
template<typename T>
void Server::resizeVector(std::size_t currSize, std::vector<T>& vectorToResize) {
  if (vectorToResize.size() <= currSize)
    vectorToResize.reserve(vectorToResize.size() * 2);
}

std::vector<Client>::iterator Server::clientItFromFd(int fd) {
  for (std::vector<Client>::iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
    if (it->getClientFd() == fd)
      return it;
  }
  return _clientsVector.end();
}

std::vector<Client> &Server::getClientsVector() {
  return _clientsVector;
}

bool Server::isClientRegistered (const std::string & clientNickName) const {
  for (std::vector<Client>::const_iterator it  = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
    if (it->getClientNickName() == clientNickName)
      return true;
  }
  return false;
}

bool Server::isChannelRegistered(const std::string & channelName) const {
  for (std::vector<Channel>::const_iterator it  = _channelsVector.begin(); it != _channelsVector.end(); ++it) {
    if (it->getName() == channelName)
      return true;
  }
  return false;
}

bool Server::isClientOnChannel(const std::string & clientNickName, std::string & channelName) const {
  if (isClientRegistered(clientNickName) && isChannelRegistered(channelName)) {
    std::set<int> channelClients = getChannelClients(channelName);
    if ((!channelClients.empty()) && (channelClients.find(getClientIdFromNickname(clientNickName)) != channelClients.end()))
      return true;
  }
  return false;
}

bool Server::isClientOperOnChannel(const std::string & clientNickName, std::string & channelName) const {
  if (isClientRegistered(clientNickName) && isChannelRegistered(channelName)) {
    std::set<int> channelOpers = getChannelOpers(channelName);
    if ((!channelOpers.empty()) && (channelOpers.find(getClientIdFromNickname(clientNickName)) != channelOpers.end()))
      return true;
  }
  return false;
}

std::set<int> Server::getChannelClients(const std::string & channelName) const {

  for (std::vector<Channel>::const_iterator it = _channelsVector.begin(); it != _channelsVector.end(); ++it) {
    if (it->getName() == channelName)
      return it->getClientsById();
  }

  return std::set<int>();

}

std::set<int> Server::getChannelOpers(const std::string & channelName) const {

  for (std::vector<Channel>::const_iterator it = _channelsVector.begin(); it != _channelsVector.end(); ++it) {
    if (it->getName() == channelName)
      return it->getOperatorsById();
  }

  return std::set<int>();
}

int Server::getClientIdFromNickname(const std::string & clientNickName) const {
  for (std::vector<Client>::const_iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
    if (it->getClientNickName() == clientNickName)
      return it->getClientId();
  }
  return -1;
}

const Client * Server::getClientInstFromId(int clientId) const {
  for (std::vector<Client>::const_iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
    if (it->getClientId() == clientId)
      return &(*it);
  }
  return NULL;
}

const Client * Server::getClientInstFromNick(const std::string & nickName) const {
  for (std::vector<Client>::const_iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
    if (it->getClientNickName() == nickName)
      return &(*it);
  }
  return NULL;
}

void Server::createChannel(const std::string& name, Client& client)
{
  _debug.debugPrint("[Server] Creating channel: " + name, GREEN);
  _channelsVector.push_back(Channel(name, _channelsVector.size() + 1, client.getClientId(), _debug));
  _debug.debugPrint("[Server] Channel " + name + " created", GREEN);
}

void Server::sendMessage(int fd, const std::string& message) {
  send(fd, message.c_str(), message.length(), 0);
}

Server* Server::instance = NULL;
