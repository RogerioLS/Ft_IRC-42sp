/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:21 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 12:32:53 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

void TopicCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  if (args.empty()) {
    debug.debugPrint("TOPIC command missing arguments", YELLOW);
    return(server.sendMessage(client.getClientFd(), Messages::ERR_NEEDMOREPARAMS(client.getClientNickName(), "TOPIC")));
  }

  const std::string& channelName = args[0];
  Channel* channel = server.getChannelByName(channelName);

  if (!channel) {
    debug.debugPrint("Channel " + channelName + " not found", YELLOW);
    return(server.sendMessage(client.getClientFd(), Messages::ERR_NOSUCHCHANNEL(client.getClientNickName(), "TOPIC")));
  }

  if (args.size() == 1) {
    // Ver o tópico
    const std::string& topic = channel->getTopic();
    std::string topic_message;
    if (topic.empty()) {
      topic_message = ":" + server.getServerName() + " 331 " + client.getClientNickName() + " " + channelName + " :No topic is set\r\n";
      debug.debugPrint("No topic set for channel " + channelName, CYAN);
    } else {
      topic_message = ":" + server.getServerName() + " 332 " + client.getClientNickName() + " " + channelName + " :" + topic + "\r\n";
      debug.debugPrint("Topic for channel " + channelName + " is: " + topic, CYAN);
    }
    server.sendMessage(client.getClientFd(), topic_message);
  } else {
    // Definir o tópico
    if (channel->getRestrictTopic()) {
      const std::set<int>& operators = channel->getOperatorsById();
      if (operators.find(client.getClientId()) == operators.end()) {
        debug.debugPrint("Client " + client.getClientNickName() + " is not an operator of " + channelName, YELLOW);
        return (server.sendMessage(client.getClientFd(), Messages::ERR_CHANOPRIVSNEEDED(client.getClientNickName(), channelName)));
      }
    } else {
      if (!client.isFullyRegistered()) {
        debug.debugPrint("Client " + client.getClientNickName() + " is not fully registered", YELLOW);
        return (server.sendMessage(client.getClientFd(), Messages::ERR_NOTREGISTERED(client.getClientNickName())));
      }
    }

    std::string newTopic;
    for (size_t i = 1; i < args.size(); ++i) {
      if (i > 1) newTopic += " ";
      newTopic += args[i];
    }
    if (newTopic[0] == ':') {
      newTopic = newTopic.substr(1);
    }

    channel->setTopic(newTopic);
    debug.debugPrint("Client " + client.getClientNickName() + " set topic for " + channelName + " to: " + newTopic, MAGENTA);

    std::string topic_msg_broadcast = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    const std::set<int>& clientIds = channel->getClientsById();
    for (std::set<int>::const_iterator it = clientIds.begin(); it != clientIds.end(); ++it) {
      const Client* destClient = server.getClientInstFromId(*it);
      if (destClient) {
        server.sendMessage(destClient->getClientFd(), topic_msg_broadcast);
      }
    }
  }
}
