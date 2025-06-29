/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:48 by codespace         #+#    #+#             */
/*   Updated: 2025/06/28 12:39:21 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Client.hpp"

Client::Client(int fd, int id) : _fd(fd), _id(id) {

	std::cout << CYAN << "Client constructor called. " <<
	"Fd: " << getClientFd() << " Id: " << getClientId() << RESET <<
	std::endl;

}

Client::~Client() {}

// Getters
int Client::getClientFd() const { return _fd; }
int Client::getClientId() const { return _id; }
bool Client::getClientIsAuthenticated() const { return _isAuthenticated; }
std::string Client::getClientNickName() const { return _nickName; }
std::string Client::getClientRealName() const { return _realName; }
std::string Client::getClientUserName() const { return _userName; }
std::string Client::getClientHostName() const { return _hostName; }

// Setters
void Client::setClientFd(int fd) { _fd = fd ;}
void Client::setClientId(int id) { _id = id ;}
void Client::setClientIsAuthenticated(bool isAuthenticated) { _isAuthenticated = isAuthenticated ;}
void Client::setClientNickName(std::string nickName) { _nickName = nickName ;}
void Client::setClientRealName(std::string realName) { _realName = realName ;}
void Client::setClientUserName(std::string userName) { _userName = userName ;}
void Client::setClientHostName(std::string hostName) { _hostName = hostName ;}
