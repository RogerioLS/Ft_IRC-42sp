/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHelper.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:39:38 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/07/15 12:19:10 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"

void CommandHandler::sendResponse(Client &client, const std::string &response) {
    send(client.getClientFd(), response.c_str(), response.length(), 0);
}

bool CommandHandler::isValidNickname(const std::string &nickname) {
    if (nickname.empty() || nickname.length() > 9) {
        return false;
    }

    if (!std::isalpha(nickname[0])) {
        return false;
    }

    for (size_t i = 1; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!std::isalnum(c) && c != '_' && c != '-' && c != '[' && c != ']' && c != '\\' && c != '`' && c != '^' && c != '{' && c != '}') {
            return false;
        }
    }
    return true;
}

bool CommandHandler::isNicknameInUse(const std::string &nickname, Server &server) {
    std::vector<Client> &clients = server.getClientsVector();
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getClientNickName() == nickname && it->hasValidNick()) {
            return true;
        }
    }
    return false;
}

bool CommandHandler::executeOperInvite(Client &client, const std::vector<std::string> &args, Server &server) {
  std::string providedClientToInvite = args[1];
  std::string providedChannel = args[2];
  std::string clientNick = client.getClientNickName();

  if (!server.isClientRegistered(providedClientToInvite))
    return(sendResponse(client, "401 : OPER :<nickname> :No such nick/channel\r\n"), false);
  if (!server.isClientOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "442 : OPER : <channel> :You're not on that channel\r\n"), false);
  if (!server.isClientOperOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "482 : OPER : <channel> : :You're not channel operator\r\n"), false);
  if (!server.isClientOnChannel(providedClientToInvite, providedChannel))
    return(sendResponse(client, "443: OPER : <user> <channel> :is already on channel\r\n"), false);
  if (!inviteClientToChannel(server, providedChannel, providedClientToInvite))
    return(sendResponse(client, "Unknown Error while Inviting Client\r\n"), false);

  sendResponse(client, "341 : OPER : <channel> <nick>\r\n");
  return true;
}

bool CommandHandler::inviteClientToChannel(Server &server, const std::string & providedChannel, const std::string & providedClientToInvite ) {
  std::vector<Channel>::iterator it = server.getChannelsVector().begin();
  while(it != server.getChannelsVector().end()) {
  if (it->getName() == providedChannel) {
    int providedClientToInviteId = server.getClientIdFromNickname(providedClientToInvite);
    it->setClientsInvitedById(providedClientToInviteId);
    sendResponse(*const_cast<Client*>(server.getClientInstFromId(providedClientToInviteId)), "You have been invited to #test by pedr");
    return true;
    }
  }
  return false;
}
