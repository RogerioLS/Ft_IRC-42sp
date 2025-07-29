/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:54:00 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 12:09:12 by pmelo-ca         ###   ########.fr       */
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
        it->setClientsInvitedById(clientInstance->getClientId());
        server.sendMessage(clientInstance->getClientFd(), "You have been invited to " +  providedChannel +  " by " + providedOper);
        return true;
      }
      ++it;
    }
    return false;
  }
}

void InviteCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  std::string clientNick = client.getClientNickName();
  int clientFd = client.getClientFd();

  if (args.size() < 3)
    return(server.sendMessage(clientFd, Messages::ERR_NEEDMOREPARAMS(clientNick, "INVITE")));

  std::string providedClientToInvite = args[1];
  std::string providedChannel = args[2];
  static_cast<void>(debug);

  if (!server.isClientRegistered(providedClientToInvite))
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHNICK(clientNick, "INVITE")));

  if(!server.isChannelRegistered(providedChannel))
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHCHANNEL(clientNick, "INVITE")));

  if (!server.isClientOnChannel(clientNick, providedChannel))
    return(server.sendMessage(clientFd, Messages::ERR_NOTONCHANNEL(clientNick, "INVITE")));

  if (!server.isClientOperOnChannel(clientNick, providedChannel))
    return(server.sendMessage(clientFd, Messages::ERR_CHANOPRIVSNEEDED(clientNick, "INVITE")));

  if (!server.isClientOnChannel(providedClientToInvite, providedChannel))
    return(server.sendMessage(clientFd, Messages::ERR_USERONCHANNEL(providedClientToInvite, "INVITE", providedChannel)));

  if (!inviteClientToChannel(server, providedChannel, providedClientToInvite, clientNick))
    return(server.sendMessage(clientFd, "Unknown Error while Inviting Client\r\n"));

  server.sendMessage(clientFd, Messages::RPL_INVITING(clientNick, providedChannel, providedClientToInvite));

}
