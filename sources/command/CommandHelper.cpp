/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHelper.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:39:38 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/07/15 11:41:30 by pmelo-ca         ###   ########.fr       */
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
