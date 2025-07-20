/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:51 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 17:18:40 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"

CommandHandler::CommandHandler() {}

void CommandHandler::processCommand(Client &client, const std::string &command, Server &server) {
	std::vector<std::string> tokens = Parser::splitCommand(command);

	if (tokens.empty()) return;

	std::string cmd = tokens[0];

	for (size_t i = 0; i < cmd.length(); ++i) {
		cmd[i] = std::toupper(cmd[i]);
	}

	if (cmd == "PASS") {
		handlePass(client, tokens);
	} else if (cmd == "NICK") {
		handleNick(client, tokens, server);
	} else if (cmd == "USER") {
		handleUser(client, tokens);
	}
}
