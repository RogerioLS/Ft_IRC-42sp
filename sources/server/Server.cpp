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

Server::Server(char **argv)
 : _port(std::atoi(argv[1])), _password(argv[2]), _parsedCommand(""), _serverFd(-1) {}

Server::~Server() {

  if (getServerFd() != -1) {
    close(getServerFd());
  }

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

// Getters
int		Server::getPort() const { return _port; }
const std::string& Server::getPassword() const { return _password; }
int		Server::getServerFd() const { return _serverFd; }
const std::string& Server::getParsedCommand() const { return _parsedCommand; }

// Setters
void Server::setParsedCommand(const std::string& cmd) { _parsedCommand = cmd; }
void Server::setServerFd(int serverFd) { _serverFd = serverFd; }