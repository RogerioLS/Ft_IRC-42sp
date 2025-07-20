/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:22:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 17:27:57 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"
#include "../includes/server/Client.hpp"

Server::Server(char **argv)
 : _port(std::atoi(argv[1])),
  _password(argv[2]),
  _serverFd(-1),
	_epollFd(-1),
	_gSignalStatus(-1) { instance = this; }

Server::~Server() { closeFds(); }

void Server::setupServer() {
	handleSignal();
	setupServerSocket();
	setupEpollEvent();
	setupClientVector();
	setupEpollLoop();
}

void Server::handleSignal() {
	signal(SIGINT, handleSigInt);
}

void Server::handleSigInt(int signum) {
	if (instance)
		instance->_gSignalStatus = signum;
}

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

		std::cout << GREEN << "Client connected. " << "fd: " << conn_socket <<  RESET << std::endl;
		int clientId = getClientCount();
		_clientsVector.push_back(Client(conn_socket, clientId));

		if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, conn_socket, &client_event) < 0) {
			std::cerr << YELLOW << ("Error epoll listen client fd") << RESET << std::endl;
			return;
		}
		std::cout << BOLD << "Client count: " << getClientCount() <<  RESET << std::endl;
	}
}

void Server::handleClientRequest(int clientFd) {

	std::vector<Client>::iterator it = clientItFromFd(clientFd);
	if (it != _clientsVector.end()) {
		std::cout << MAGENTA << "Client interacted." << " fd: " << clientFd << RESET << std::endl;
		char buffer[1024];
		ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer), 0);

		if (bytesRead <= 0) {
			std::cerr << YELLOW << "Client id: " << it->getClientId() << " disconnected" << RESET << std::endl;
			close(it->getClientFd());
			_clientsVector.erase(it);
		}

		buffer[bytesRead] = '\0';
		it->setClientBuffer(buffer);
		Parser::parseBuffer(*it);
		//execute;
	}
}

// Getters
int		Server::getPort() const { return _port; }
const std::string& Server::getPassword() const { return _password; }
int		Server::getServerFd() const { return _serverFd; }
int		Server::getEpollFd() const { return _epollFd; }
int		Server::getClientCount() const { return _clientsVector.size(); }
int	Server:: getServerRunning() const { return _gSignalStatus; }

// Setters
void Server::setServerFd(int serverFd) { _serverFd = serverFd; }
void Server::setEpollFd(int epollFd) { _epollFd = epollFd; }
void Server::setServerRunning(int gSignalStatus) { _gSignalStatus = gSignalStatus; }

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

Server* Server::instance = NULL;
