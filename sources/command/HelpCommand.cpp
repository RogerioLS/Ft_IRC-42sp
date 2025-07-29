/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HelpCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:09 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:39:41 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

void HelpCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  (void)args;
  debug.debugPrint("Executing HELP command for client " + client.getClientNickName(), MAGENTA);

  std::string serverName = server.getServerName();
  std::string nickName = client.getClientNickName();

  std::string help_message = ":" + serverName + " NOTICE " + nickName + " :Available commands:\r\n";
  server.sendMessage(client.getClientFd(), help_message);

  help_message = ":" + serverName + " NOTICE " + nickName + " :  JOIN <#channel> - Join or create a channel.\r\n";
  server.sendMessage(client.getClientFd(), help_message);

  help_message = ":" + serverName + " NOTICE " + nickName + " :  PRIVMSG <target> :<message> - Send a private message to a user or channel.\r\n";
  server.sendMessage(client.getClientFd(), help_message);

  help_message = ":" + serverName + " NOTICE " + nickName + " :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel.\r\n";
  server.sendMessage(client.getClientFd(), help_message);

  help_message = ":" + serverName + " NOTICE " + nickName + " :  HELP - Shows this help message.\r\n";
  server.sendMessage(client.getClientFd(), help_message);
}
