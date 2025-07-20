/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:52:02 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/07/20 14:55:36 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"

void CommandHandler::handleUser(Client &client, const std::vector<std::string> &args) {
	if (args.size() < 5) {
		sendResponse(client, "461 USER :Not enough parameters\r\n");
		return;
	}

	if (client.hasValidUser()) {
		sendResponse(client, "462 :You may not reregister\r\n");
		return;
	}

	std::string username = args[1];
	std::string realname = args[4];

	if (!realname.empty() && realname[0] == ':') {
		realname = realname.substr(1);
	}

	client.setClientUserName(username);
	client.setClientRealName(realname);
	client.setHasValidUser(true);

	std::cout << GREEN << "Client " << client.getClientFd() << " set user info: " << username << " (" << realname << ")" RESET << std::endl;
}
