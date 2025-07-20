/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:51:39 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/07/20 16:27:04 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"

void CommandHandler::handlePass(Client &client, const std::vector<std::string> &args) {
	if (args.size() < 2) {
		sendResponse(client, "461 PASS :Not enough parameters\r\n");
		return;
	}

	if (client.isFullyRegistered()) {
		sendResponse(client, "462 :You may not reregister\r\n");
		return;
	}

	std::string providedPassword = args[1];
	std::cout << GREEN << "Client " << client.getClientFd() << " provided correct password" << RESET << std::endl;
	
}
