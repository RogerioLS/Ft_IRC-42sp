/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:45 by codespace         #+#    #+#             */
/*   Updated: 2025/07/30 13:25:45 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/Channel.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/server/Client.hpp"

Channel::Channel(std::string name, int channelId, int clientId, Debug& debug)
  : _inviteOnly(false),
    _restrictTopic(true),
    _userLimit(0),
    _channelId(channelId),
    _name(name),
    _topic(""),
    _key(""),
    _operatorsById(std::set<int>()),
    _clientsById(std::set<int>()),
    _clientsInvitedById(std::set<int>()),
    _debug(debug)
  {
    _debug.debugPrint("[Channel] Creating channel: " + _name, GREEN);
    setOperatorsById(clientId);
    setClientsById(clientId);
  }

  Channel::~Channel() {}

    // Getters
bool Channel::getInviteOnly() const { return _inviteOnly; }
bool Channel::getRestrictTopic() const { return _restrictTopic; }
int Channel::getUserLimit() const { return _userLimit; }
int Channel::getChannelId() const { return _channelId; }
const std::string & Channel::getName() const { return _name; }
const std::string & Channel::getTopic() const { return _topic; }
const std::string & Channel::getKey() const { return _key; }
const std::set<int> & Channel::getOperatorsById() const { return _operatorsById; }
const std::set<int> & Channel::getClientsById() const { return _clientsById; }
const std::set<int> & Channel::getInvitedById() const { return _clientsInvitedById; }

// Setters
void Channel::setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly; }
void Channel::setRestrictTopic(bool restrictTopic) { _restrictTopic = restrictTopic; }
void Channel::setUserLimit(int userLimit) { _userLimit = userLimit; }
void Channel::setChannelId(int channelId) { _channelId = channelId; }
void Channel::setName(const std::string & name) { _name = name; }
void Channel::setTopic(const std::string & topic) { _topic = topic; }
void Channel::setKey(const std::string & key) { _key = key; }

void Channel::setOperatorsById(int operatorId) {
  std::stringstream ss_op;
  ss_op << operatorId;
  _debug.debugPrint("[Channel] Adding operator " + ss_op.str() + " to channel " + _name, CYAN);
  _operatorsById.insert(operatorId);
}

void Channel::setClientsById(int clientId) {
  std::stringstream ss_cl;
  ss_cl << clientId;
  _debug.debugPrint("[Channel] Adding client " + ss_cl.str() + " to channel " + _name, CYAN);
  this->_clientsById.insert(clientId);
}

bool Channel::isClientInChannel(int clientId) const { return (_clientsById.count(clientId) > 0); }
void Channel::setClientsInvitedById(int clientId) { _clientsInvitedById.insert(clientId); }
void Channel::removeClient(int cliendId) { _clientsById.erase(cliendId); }
void Channel::removeOper(int operId) { _operatorsById.erase(operId); }
void Channel::addOper(int operId) { _operatorsById.insert(operId); }

void Channel::broadcastToAll(Server & server,const std::string& message) {
  for (std::set<int>::iterator it = _clientsById.begin(); it != _clientsById.end(); ++it) {
    const Client * client = server.getClientInstFromId(*it);
    if (client)
      send(client->getClientFd(), message.c_str(), message.length(), 0);
  }
}
