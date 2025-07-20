/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:51 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 19:21:52 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"

void CommandHandler::handlePass(Client &client, const std::vector<std::string> &args, Server &server) {
	if (args.size() < 2) {
		sendResponse(client, "461 PASS :Not enough parameters\r\n");
		return;
	}

	if (client.isFullyRegistered()) {
		sendResponse(client, "462 :You may not reregister\r\n");
		return;
	}

	std::string providedPassword = args[1];
	if (providedPassword == server.getPassword()) {
		client.setClientPassword(providedPassword);
		client.setHasValidPass(true);
		std::cout << GREEN << "Client " << client.getClientFd() << " provided correct password" << RESET << std::endl;
	} 
	else {
		client.setHasValidPass(false);
		sendResponse(client, "464 :Password incorrect\r\n");
	}
}
