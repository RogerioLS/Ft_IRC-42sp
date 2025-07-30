/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:54:00 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/30 10:44:15 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

namespace {

  bool inviteClientToChannel(Server &server, const std::string & providedChannel, const std::string & providedClientToInvite, const std::string & providedOper ) {

    std::vector<Channel>::iterator it = server.getChannelsVector().begin();
    while(it != server.getChannelsVector().end()) {
      if (it->getName() == providedChannel) {
        const Client * clientInstance = server.getClientInstFromNick(providedClientToInvite);
        if (!clientInstance) {
          server.getDebug().debugPrint("[INVITE] Client instance not found for: " + providedClientToInvite, RED);
          return false;
        }
        it->setClientsInvitedById(clientInstance->getClientId());
        server.getDebug().debugPrint("[INVITE] " + providedClientToInvite + " invited to " + providedChannel + " by " + providedOper, GREEN);
        server.sendMessage(clientInstance->getClientFd(), "You have been invited to " +  providedChannel +  " by " + providedOper);
        return true;
      }
      ++it;
    }
    server.getDebug().debugPrint("[INVITE] Channel not found: " + providedChannel, RED);
    return false;
  }
}

void InviteCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  std::string clientNick = client.getClientNickName();
  int clientFd = client.getClientFd();

  if (args.size() < 3) {
    debug.debugPrint("[INVITE] Not enough parameters", YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NEEDMOREPARAMS(clientNick, "INVITE")));
  }

  std::string providedClientToInvite = args[1];
  std::string providedChannel = args[2];
  static_cast<void>(debug);

  if (!server.isClientRegistered(providedClientToInvite)) {
    debug.debugPrint("[INVITE] Target nick not registered: " + providedClientToInvite, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHNICK(clientNick, "INVITE")));
  }

  if(!server.isChannelRegistered(providedChannel)) {
    debug.debugPrint("[INVITE] Channel not registered: " + providedChannel, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHCHANNEL(clientNick, "INVITE")));
  }

  if (!server.isClientOnChannel(clientNick, providedChannel)) {
    debug.debugPrint("[INVITE] Client not on channel: " + providedChannel, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NOTONCHANNEL(clientNick, "INVITE")));
  }

  if (!server.isClientOperOnChannel(clientNick, providedChannel)) {
    debug.debugPrint("[INVITE] Client is not channel operator: " + clientNick, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_CHANOPRIVSNEEDED(clientNick, "INVITE")));
  }

  if (!server.isClientOnChannel(providedClientToInvite, providedChannel)) {
    debug.debugPrint("[INVITE] Target already on channel: " + providedClientToInvite + " " + providedChannel, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_USERONCHANNEL(providedClientToInvite, "INVITE", providedChannel)));
  }

  if (!inviteClientToChannel(server, providedChannel, providedClientToInvite, clientNick)) {
    debug.debugPrint("[INVITE] Unknown error while inviting " + providedClientToInvite, RED);
    return(server.sendMessage(clientFd, "Unknown Error while Inviting Client\r\n"));
  }

  debug.debugPrint("[INVITE] Invitation successful: " + providedClientToInvite + " to " + providedChannel, GREEN);
  server.sendMessage(clientFd, Messages::RPL_INVITING(clientNick, providedChannel, providedClientToInvite));
}
