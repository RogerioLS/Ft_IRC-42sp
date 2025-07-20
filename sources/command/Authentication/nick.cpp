/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:51:32 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/07/20 18:53:05 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"

void CommandHandler::handleNick(Client &client, const std::vector<std::string> &args, Server &server) {
	if (args.size() < 2) {
		sendResponse(client, "431, :No nickname given \r\n");
		return;
	}

	std::string nickname = args[1];

	if (!isValidNickname(nickname)) {
		sendResponse(client, "432 " + nickname + " :Erroneous nickname \r\n");
		return;
	}

	if (isNicknameInUse(nickname, server)) {
		sendResponse(client, "433 " + nickname + " :Nickname is already in use \r\n");
		return;
	}

	client.setClientNickName(nickname);
	client.setHasValidNick(true);

    std::string reply = nickname + " :Welcome to the Internet Relay Network\r\n";
    send(client.getClientFd(), reply.c_str(), reply.length(), 0);
	std::cout << GREEN << "Client " << client.getClientFd() << " set nickname to: " << nickname << RESET << std::endl;
}