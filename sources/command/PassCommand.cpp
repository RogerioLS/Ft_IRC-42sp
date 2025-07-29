/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:16 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:34:17 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

void PassCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    if (client.isFullyRegistered()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_ALREADYREGISTRED(client.getClientNickName()));
        debug.debugPrint("[PASS] Client already registered.", YELLOW);
        return;
    }

    if (args.empty()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_NEEDMOREPARAMS(client.getClientNickName(), "PASS"));
        debug.debugPrint("[PASS] Missing password argument.", YELLOW);
        return;
    }

    const std::string& password = args[0];
    if (password == server.getPassword()) {
        client.setHasValidPass(true);
        debug.debugPrint("[PASS] Client authenticated successfully.", GREEN);
    } else {
        server.sendMessage(client.getClientFd(), Messages::ERR_PASSWDMISMATCH(client.getClientNickName()));
        debug.debugPrint("[PASS] Incorrect password attempt.", YELLOW);
    }
}
