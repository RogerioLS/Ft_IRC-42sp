/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:54:02 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/08/04 20:09:14 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"
#include "../includes/parser/Parser.hpp"

namespace {
  bool kickClientFromChannel(Server &server, const std::string &providedChannel, const std::string &providedClientToKick, const std::string & providedOper, const std::string & reasonToKick) {
  std::vector<Channel>::iterator it = server.getChannelsVector().begin();

  while (it != server.getChannelsVector().end()) {
    if (it->getName() == providedChannel) {
      const Client * clientInstance = server.getClientInstFromNick(providedClientToKick);
      if (!clientInstance) {
        server.getDebug().debugPrint("[KICK] Client instance not found for: " + providedClientToKick, RED);
        return false;
      }
      it->removeClient(clientInstance->getClientId());

      if (it->getOperatorsById().find(clientInstance->getClientId()) != it->getOperatorsById().end()) {
        it->removeOper(clientInstance->getClientId());
        server.getDebug().debugPrint("[KICK] Removed operator from " + providedClientToKick + " from " + providedChannel , CYAN);
      }
      server.getDebug().debugPrint("[KICK] Sending kick message to " + providedClientToKick + " from " + providedChannel + " reason" + reasonToKick, CYAN);
      server.sendMessage(clientInstance->getClientFd(),  "You have been kicked from " + providedChannel + " by " + providedOper + "(" + providedChannel + reasonToKick + ")" + "\r\n");
      it->broadcastToAll(server, ":" + clientInstance->getClientNickName() + " KICK " + providedChannel + " " + providedClientToKick + " :" + reasonToKick + "\r\n");
      return true;
    }
    ++it;
  }
  server.getDebug().debugPrint("[KICK] Channel not found: " + providedChannel, RED);
  return false;
  }
}

void KickCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  std::string clientNick = client.getClientNickName();
  int clientFd = client.getClientFd();

  debug.debugPrint("[KICK] Command received from: " + clientNick, CYAN);
  for (size_t i = 0; i < args.size(); ++i) {
    std::stringstream ss;
    ss << i;
    debug.debugPrint("[KICK] Mode arg[" + ss.str() + "]: " + args[i], GREEN);
  }

  if (args.size() < 2) {
    debug.debugPrint("[KICK] Not enough parameters", YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NEEDMOREPARAMS(clientNick, "KICK")));
  }

  std::string reasonToKick;
  std::vector<std::string> channelsFromKick = Parser::splitCommandArg(args[0], ",");
  std::vector<std::string> clientsToKick = Parser::splitCommandArg(args[1], ",");

  if (args.size() > 2) {
    for (size_t i = 2; i < args.size(); ++i) {
      reasonToKick += " ";
      reasonToKick += args[i];
    }
  }
  else
    reasonToKick = "";

  for (size_t i = 0; i < channelsFromKick.size(); ++i) {
    std::string providedChannel = channelsFromKick[i];
    if (!server.isChannelRegistered(providedChannel)) {
      debug.debugPrint("[KICK] Channel not registered: " + providedChannel, YELLOW);
      server.sendMessage(clientFd, Messages::ERR_NOSUCHCHANNEL(clientNick, providedChannel));
      continue;
    }

    if (!server.isClientOnChannel(clientNick, providedChannel)) {
      debug.debugPrint("[KICK] Client not on channel: " + providedChannel, YELLOW);
      server.sendMessage(clientFd, Messages::ERR_NOTONCHANNEL(clientNick, providedChannel));
      continue;
    }

    if (!server.isClientOperOnChannel(clientNick, providedChannel)) {
      debug.debugPrint("[KICK] Client is not channel operator: " + clientNick, YELLOW);
      server.sendMessage(clientFd, Messages::ERR_CHANOPRIVSNEEDED(clientNick, providedChannel));
      continue;
    }

    for (size_t j = 0; j < clientsToKick.size(); ++j) {
      std::string providedClientToKick = clientsToKick[j];
      if (!client.isFullyRegistered()) {
        debug.debugPrint("[KICK] Client not registered: " + clientNick, YELLOW);
        server.sendMessage(clientFd, Messages::ERR_NOTREGISTERED(clientNick));
        continue;
      }
      if (!server.isClientFullyRegistered(providedClientToKick)) {
        debug.debugPrint("[KICK] Target nick not registered: " + providedClientToKick, YELLOW);
        return(server.sendMessage(clientFd, Messages::ERR_NOSUCHNICK(clientNick, providedClientToKick)));
      }
      if (!server.isClientOnChannel(providedClientToKick, providedChannel)) {
        debug.debugPrint("[KICK] Target not on channel: " + providedClientToKick + " " + providedChannel, YELLOW);
        server.sendMessage(clientFd, Messages::ERR_USERNOTINCHANNEL(providedClientToKick, providedChannel));
        continue;
      }
      if (!kickClientFromChannel(server, providedChannel, providedClientToKick, clientNick, reasonToKick)) {
        debug.debugPrint("[KICK] Unknown error kicking " + providedClientToKick, RED);
        server.sendMessage(clientFd, "Unknown error kicking " + providedClientToKick + "\r\n");
        continue;
      }
      debug.debugPrint("[KICK] KICK successful for " + providedClientToKick + " on " + providedChannel, GREEN);
      server.sendMessage(clientFd, "KICK successful for " + providedClientToKick + " on " + providedChannel + "\r\n");
    }
  }
}
