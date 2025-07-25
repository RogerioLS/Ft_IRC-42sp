/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:45 by codespace         #+#    #+#             */
/*   Updated: 2025/07/01 11:47:03 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/Channel.hpp"
#include "../../includes/utils/Colors.hpp"
#include <sstream>

		Channel::Channel(std::string name, int channelId, int clientId, Debug& debug)
		:	_inviteOnly(false),
			_restrictTopic(true),
			_userLimit(0),
			_channelId(channelId),
			_name(name),
			_topic(""),
			_key(""),
			_operatorsById(std::set<int>()),
			_clientsById(std::set<int>()),
			_debug(debug)
		{
			_debug.debugPrint("[Channel] Creating channel: " + _name, GREEN);
			setOperatorsById(clientId);
			setClientsById(clientId);
		}

		Channel::~Channel() {
			_debug.debugPrint("[Channel] Destroying channel: " + _name, RED);
			_operatorsById.clear();
			_clientsById.clear();
		}

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
		void Channel::setClientsById(int clientId)
{
	std::stringstream ss_cl;
	ss_cl << clientId;
	_debug.debugPrint("[Channel] Adding client " + ss_cl.str() + " to channel " + _name, CYAN);
	this->_clientsById.insert(clientId);
}

bool Channel::isClientInChannel(int clientId) const {
    return (_clientsById.count(clientId) > 0);
}
