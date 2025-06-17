/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:22:00 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 19:22:02 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"

Server::Server(char **argv) :
	_port(std::atoi(argv[1])), _password(argv[2]), _parsedCommand("") {}

Server::~Server() {

  // close(getServerFd());
  // close(getEpollFd());

}

// Epoll management
void Server::setupServerSocket() {
	// throw std::runtime_error("Erro ao criar socket");
}

void Server::setupEpoll() {
	// throw std::runtime_error("Erro ao criar socket");
}

void Server::startEpoolLoop() {
	// throw std::runtime_error("Erro ao criar socket");
}

// Getters
int		Server::getPort() const { return _port; }
const std::string& Server::getPassword() const { return _password; }
int		Server::getServerFd() const { return _serverFd; }
int		Server::getEpollFd() const { return _epollFd; }
const std::string& Server::getParsedCommand() const { return _parsedCommand; }

// Setters
void Server::setParsedCommand(const std::string& cmd) { _parsedCommand = cmd; }