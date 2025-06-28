/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:22:00 by codespace         #+#    #+#             */
/*   Updated: 2025/06/28 15:12:49 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"

Server::Server(char **argv)
 : _port(std::atoi(argv[1])), _password(argv[2]), _parsedCommand(""),
 _serverFd(-1), _epollFd(-1), _clientCount(0), _running(true) {}

Server::~Server() {

	if (getServerFd() != -1) {
		close(getServerFd());
	}

	if (getEpollFd() != -1) {
		close(getEpollFd());
	}
}

void Server::setupServer() {
	setupServerSocket();
	setupEpollEvent();
	setupEpollLoop();
	setupClientVector();
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
}

void Server::setupEpollLoop() {

	while (getServerRunning() == true) {

		int nfds = epoll_wait(_epollFd, _eventsVector.data(), _eventsVector.size(), 0);
		if (nfds < 0)
			throw std::runtime_error("Error epoll_wait");

		for (int n = 0; n < nfds; ++n) {
			if (_eventsVector[n].data.fd == getServerFd())
				handleNewClient();
			else
				handleClientRequest(n);
		}
		resizeVector(static_cast<std::size_t>(nfds), _eventsVector);
		resizeVector(_clientsVector.size(), _clientsVector);
	}
}

void Server::setupClientVector() {
	if (_clientsVector.empty())
		_clientsVector.reserve(INITIAL_CLIENT_VECTOR_SIZE);
}

void Server::handleNewClient() {
	sockaddr_in client_addr;
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

		if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, conn_socket, &client_event) < 0) {
			std::cerr << YELLOW << ("Error epoll listen client fd") << RESET << std::endl;
			return;
		}
		std::cout << GREEN << "Client connected. " << "fd: " << conn_socket <<  RESET << std::endl;
		int clientId = getClientCount() + 1;
		_clientsVector.push_back(Client(conn_socket, clientId));
		setClientCount(clientId);
		std::cout << BOLD << "Client count: " << getClientCount() <<  RESET << std::endl;
	}
}

void Server::handleClientRequest(int n) {
	std::cout << MAGENTA << "Client interacted." << " fd: " << _eventsVector[n].data.fd << RESET << std::endl;
}

// Getters
int		Server::getPort() const { return _port; }
const std::string& Server::getPassword() const { return _password; }
int		Server::getServerFd() const { return _serverFd; }
const std::string& Server::getParsedCommand() const { return _parsedCommand; }
int		Server::getEpollFd() const { return _epollFd; }
int		Server::getClientCount() const { return _clientCount; }
bool	Server:: getServerRunning() const { return _running; }
const std::vector<struct epoll_event>& Server::getEvents() const { return _eventsVector; }

// Setters
void Server::setParsedCommand(const std::string& cmd) { _parsedCommand = cmd; }
void Server::setServerFd(int serverFd) { _serverFd = serverFd; }
void Server::setEpollFd(int epollFd) { _epollFd = epollFd; }
void Server::setClientCount(int clientCount) { _clientCount = clientCount; }
void Server::setServerRunning(bool running) { _running = running; }

template<typename T>
void Server::resizeVector(std::size_t currSize, std::vector<T>& vectorToResize) {
	if (vectorToResize.size() <= currSize)
		vectorToResize.reserve(vectorToResize.size() * 2);
}
