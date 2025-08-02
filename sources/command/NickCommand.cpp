/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:13 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:34:14 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

static bool isValidNickname(const std::string& nickname) {
  if (nickname.length() > 9)
    return false;

  for (size_t i = 0; i < nickname.length(); ++i) {
    char c = nickname[i];
    if (!std::isalnum(c) && c != '_' && c != '-' && c != '[' && c != ']' && c != '\\' && c != '`' && c != '^' && c != '{' && c != '}')
      return false;
  }
  return true;
}

void NickCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  if (!client.hasValidPass()) {
    server.sendMessage(client.getClientFd(), ":server NOTICE * :You must provide a password first.\r\n");
    debug.debugPrint("[NICK] Failing because client has no valid pass.", RED);
    return;
  }

  if (args.empty()) {
    server.sendMessage(client.getClientFd(), Messages::ERR_NONICKNAMEGIVEN(client.getClientNickName()));
    debug.debugPrint("[NICK] Failing because args is empty.", RED);
    return;
  }

  const std::string& newNickname = args[0];

  if (!isValidNickname(newNickname)) {
    server.sendMessage(client.getClientFd(), Messages::ERR_ERRONEUSNICKNAME(client.getClientNickName(), newNickname));
    debug.debugPrint("[NICK] Failing because nickname is invalid.", RED);
    return;
  }

  const Client* existingClient = server.getClientInstFromNick(newNickname);
  if (existingClient != NULL && existingClient->getClientId() != client.getClientId()) {
    server.sendMessage(client.getClientFd(), Messages::ERR_NICKNAMEINUSE(client.getClientNickName(), newNickname));
    debug.debugPrint("[NICK] Failing because nickname is in use.", RED);
    return;
  }

  client.setClientNickName(newNickname);
  client.setHasValidNick(true);
  debug.debugPrint("[NICK] Nickname set successfully to: " + newNickname, GREEN);
}
