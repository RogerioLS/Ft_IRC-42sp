/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:51 by codespace         #+#    #+#             */
/*   Updated: 2025/07/23 12:26:38 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"
#include "../includes/utils/Colors.hpp"

CommandHandler::CommandHandler() {}

void CommandHandler::processCommand(Client &client, const std::string &command, Server &server) {
	std::vector<std::string> tokens = Parser::splitCommand(command);

	if (tokens.empty()) return;

	std::string cmd = tokens[0];

	for (size_t i = 0; i < cmd.length(); ++i) {
		cmd[i] = std::toupper(cmd[i]);
	}

	if (cmd == "PASS") {
		handlePass(client, tokens, server);
	} else if (cmd == "NICK") {
		handleNick(client, tokens, server);
	} else if (cmd == "USER") {
		handleUser(client, tokens);
	} else if (cmd == "KICK") {
    handleOperKick(client, tokens, server);
  } else if (cmd == "INVITE") {
    handleOperInvite(client, tokens, server);
  } else if (cmd == "MODE") {
    handleOperMode(client, tokens, server);
  }
}

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
		std::cout << GREEN << "Client " << client.getClientId() << " provided correct password" << RESET << std::endl;
	}
	else {
		client.setHasValidPass(false);
		sendResponse(client, "464 :Password incorrect\r\n");
	}
}

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
	std::cout << GREEN << "Client " << client.getClientId() << " set nickname to: " << nickname << RESET << std::endl;
}

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

	std::cout << GREEN << "Client " << client.getClientId() << " set user info: " << username << " (" << realname << ")" RESET << std::endl;
}

void CommandHandler::handleOperKick(Client &client, const std::vector<std::string> &args, Server &server) {
  if(executeOperKick(client, args, server))
    std::cout << GREEN << "At least one client was kicked from channel/channels" << RESET << std::endl;
  else
    std::cout << RED << "Clients was/were not kicked from channel/channels (" << args[2] << ")" << RESET << std::endl;
}

void CommandHandler::handleOperInvite(Client &client, const std::vector<std::string> &args, Server &server) {
  if(executeOperInvite(client, args, server))
    std::cout << GREEN << "Client " << client.getClientId() << " invited " << args[1] << "to channel: (" << args[2] << ")" << RESET << std::endl;
  else
    std::cout << RED << "Client " << args[1]<< " was not invited " << "to channel: (" << args[2] << ")" << RESET << std::endl;
}

void CommandHandler::handleOperMode(Client &client, const std::vector<std::string> &args, Server &server) {
  if(executeChannelMode(client, args, server))
    std::cout << GREEN << "Client " << client.getClientId() << " set mode to channel: (" << args[1] << ")" << RESET << std::endl;
  else
    std::cout << RED << "Client " << client.getClientId() << " does not set mode to channel: (" << args[1] << ")" << RESET << std::endl;
}
