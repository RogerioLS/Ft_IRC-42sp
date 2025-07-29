/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:11 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:34:12 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

void JoinCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  debug.debugPrint("[JOIN] Handling JOIN command", BLUE);
  if (args.empty()) {
    debug.debugPrint("[JOIN] Missing arguments", YELLOW);
    // Enviar ERR_NEEDMOREPARAMS para o cliente
    return;
  }

  const std::string& channelName = args[0];
  debug.debugPrint("[JOIN] Channel name: " + channelName, BLUE);

  // Validação simples do nome do canal
  if (channelName[0] != '#') {
    debug.debugPrint("[JOIN] Invalid channel name: " + channelName, YELLOW);
    // Enviar ERR_BADCHANMASK ou similar
    return;
  }

  debug.debugPrint("[JOIN] Client " + client.getClientNickName() + " wants to join " + channelName, MAGENTA);

  Channel* channel = server.getChannelByName(channelName);
  if (channel) {
    // Canal existe, adicionar cliente
    debug.debugPrint("[JOIN] Channel " + channelName + " already exists. Adding client.", CYAN);
    channel->setClientsById(client.getClientId());
  } else {
    // Canal não existe, criar
    debug.debugPrint("[JOIN] Channel " + channelName + " does not exist. Creating.", CYAN);
    server.createChannel(channelName, client);
  }
  debug.debugPrint("[JOIN] Finished handling JOIN command", BLUE);
}
