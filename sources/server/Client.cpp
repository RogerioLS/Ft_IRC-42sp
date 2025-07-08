/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:48 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 12:57:29 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Client.hpp"

Client::Client(int fd, int id, uint16_t port, std::string ipAddress)
 : _fd(fd), _id(id), _port(port), _isAuthenticated(false), _ipAddress(ipAddress),
   _hasValidPass(false), _hasValidNick(false), _hasValidUser(false) {}

Client::~Client() {}

// Getters
int Client::getClientFd() const { return _fd; }
int Client::getClientId() const { return _id; }
uint16_t Client::getClientPort() const { return _port; }
bool Client::getClientIsAuthenticated() const { return _isAuthenticated; }
const std::string Client::getClientPassword() const { return _password; }
const std::string Client::getClientNickName() const { return _nickName; }
const std::string Client::getClientRealName() const { return _realName; }
const std::string Client::getClientUserName() const { return _userName; }
const std::string Client::getClientipAddress() const { return _ipAddress; }
char * Client::getClientBufferChar() { return _bufferChar; }
const std::string Client::getClientBufferStr() const { return _bufferStr; }
const std::vector<std::string> Client:: getClientParsedCommand() const { return _parsedCommand; }

// Setters
void Client::setClientFd(int fd) { _fd = fd ;}
void Client::setClientId(int id) { _id = id ;}
void Client::setClientPort(uint16_t port) { _port = port; }
void Client::setClientIsAuthenticated(bool isAuthenticated) { _isAuthenticated = isAuthenticated ;}
void Client::setClientPassword(const std::string & password) { _password = password ;}
void Client::setClientNickName(const std::string & nickName) { _nickName = nickName ;}
void Client::setClientRealName(const std::string & realName) { _realName = realName ;}
void Client::setClientUserName(const std::string & userName) { _userName = userName ;}
void Client::setClientIpAddress(const std::string & ipAddress) { _ipAddress = ipAddress ;}
void Client::setClientBufferStr(const std::string & bufferStr) { _bufferStr = bufferStr ;}
void Client::setClientParsedCommand(const std::vector<std::string> & parsedCommand) { _parsedCommand = parsedCommand ;}
void Client::appendParsedCommand(const std::string & line) { _parsedCommand.push_back(line); }

bool	Client::hasValidPass() const { return _hasValidPass; }
bool	Client::hasValidNick() const { return _hasValidNick; }
bool	Client::hasValidUser() const { return _hasValidUser; }
bool	Client::isFullyRegistered() const {
	return _hasValidPass && _hasValidNick && _hasValidUser;
}

void Client::setHasValidPass(bool hasPass) {_hasValidPass = hasPass; };
void Client::setHasValidNick(bool hasNick) {_hasValidNick = hasNick; };
void Client::setHasValidUser(bool hasUser) {_hasValidUser = hasUser; };

