/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:19 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 12:36:02 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

void PrivmsgCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  if (args.size() < 2) {
    debug.debugPrint("PRIVMSG command missing arguments", YELLOW);
    return(server.sendMessage(client.getClientFd(), Messages::ERR_NEEDMOREPARAMS(client.getClientNickName(), "PRIVMSG")));
  }

  const std::string& target = args[0];
  std::string message;
  for (size_t i = 1; i < args.size(); ++i) {
    if (i > 1) message += " ";
    message += args[i];
  }

  if (message[0] == ':') {
    message = message.substr(1);
  }

  std::string fullMessage = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " PRIVMSG " + target + " :" + message + "\r\n";

  if (target[0] == '#') {
    // Enviar para canal
    debug.debugPrint("Client " + client.getClientNickName() + " sending message to channel " + target, MAGENTA);
    Channel* channel = server.getChannelByName(target);
    if (channel) {
      const std::set<int>& clientIds = channel->getClientsById();
      for (std::set<int>::const_iterator it = clientIds.begin(); it != clientIds.end(); ++it) {
        if (*it != client.getClientId()) { // Não enviar para o próprio remetente
          const Client* destClient = server.getClientInstFromId(*it);
          if (destClient) {
            server.sendMessage(destClient->getClientFd(), fullMessage);
          }
        }
      }
    } else {
      debug.debugPrint("Channel " + target + " not found", YELLOW);
      return(server.sendMessage(client.getClientFd(), Messages::ERR_NOSUCHCHANNEL(target, "PRIVMSG")));
    }
  } else {
    // Enviar para usuário
    debug.debugPrint("Client " + client.getClientNickName() + " sending message to user " + target, MAGENTA);
    const Client* destClient = server.getClientInstFromNick(target);
    if (destClient) {
      server.sendMessage(destClient->getClientFd(), fullMessage);
    } else {
      debug.debugPrint("User " + target + " not found", YELLOW);
      return(server.sendMessage(client.getClientFd(), Messages::ERR_NOSUCHNICK(target, "PRIVMSG")));
    }
  }
}
