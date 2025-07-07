/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:48 by codespace         #+#    #+#             */
/*   Updated: 2025/07/07 10:29:47 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Client.hpp"
#include "../includes/utils/Colors.hpp"

Client::Client(int fd, int id) : _fd(fd), _id(id), _isAuthenticated(false) {}

Client::~Client() {}

// Getters
int Client::getClientFd() const { return _fd; }
int Client::getClientId() const { return _id; }
bool Client::getClientIsAuthenticated() const { return _isAuthenticated; }
const std::string & Client::getClientPassword() const { return _password; }
const std::string & Client::getClientNickName() const { return _nickName; }
const std::string & Client::getClientRealName() const { return _realName; }
const std::string & Client::getClientUserName() const { return _userName; }
const std::string & Client::getClientHostName() const { return _hostName; }

// Setters
void Client::setClientFd(int fd) { _fd = fd ;}
void Client::setClientId(int id) { _id = id ;}
void Client::setClientIsAuthenticated(bool isAuthenticated) { _isAuthenticated = isAuthenticated ;}
void Client::setClientPassword(std::string password) { _password = password ;}
void Client::setClientNickName(std::string nickName) { _nickName = nickName ;}
void Client::setClientRealName(std::string realName) { _realName = realName ;}
void Client::setClientUserName(std::string userName) { _userName = userName ;}
void Client::setClientHostName(std::string hostName) { _hostName = hostName ;}
