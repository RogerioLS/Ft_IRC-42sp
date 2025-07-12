/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:51 by codespace         #+#    #+#             */
/*   Updated: 2025/07/12 16:45:22 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
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
	} else if (cmd == "/KICK") {
    handleOperKick(client, tokens, server);
  } else if (cmd == "/INVITE") {
    handleOperInvite(client, tokens, server);
  } else if (cmd == "/MODE") {
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
		std::cout << GREEN << "Client " << client.getClientFd() << " provided correct password" << RESET << std::endl;
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
	std::cout << GREEN << "Client " << client.getClientFd() << " set nickname to: " << nickname << RESET << std::endl;
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

	std::cout << GREEN << "Client " << client.getClientFd() << " set user info: " << username << " (" << realname << ")" RESET << std::endl;
}

void CommandHandler::handleOperKick(Client &client, const std::vector<std::string> &args, Server &server) {
  // if (args.size() < 5) {
	// 	sendResponse(client, "ERR_NEEDMOREPARAMS :Not enough parameters\r\n");
	// 	return;
	// }

	std::string providedChannel = args[1];
  std::string providedClientToKick = args[2];
  std::string providedComment = args[3];

  (void) client;
  (void) server;

  //check if specified channel exist: sendResponse(client, "ERR_NOSUCHCHANNEL :"<channel name> :No such channel"\r\n");
  //check if client is on channel: sendResponse(client, "442 ERR_NOTONCHANNEL : "<channel> :You're not on that channel""\r\n");
  //check if client is OP in specified channel : sendResponse(client, "ERR_CHANOPRIVSNEEDED "<channel> : :You're not channel operator\r\n");
  //check if providedClientToKick is connected to channel: sendResponse(client, " 441 ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"\r\n");
  //remove it from the Channel instance:
}

void CommandHandler::handleOperInvite(Client &client, const std::vector<std::string> &args, Server &server) {
  if (args.size() < 3)
    return (sendResponse(client, "461: OPER :Not enough parameters\r\n"));

  std::string providedClientToInvite = args[1];
  std::string providedChannel = args[2];
  std::string clientNick = client.getClientNickName();

  if (!server.isClientRegistered(providedClientToInvite))
    return(sendResponse(client, "401 : OPER :<nickname> :No such nick/channel\r\n"));
  if (!server.isClientOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "442 : OPER : <channel> :You're not on that channel\r\n"));
  if (!server.isClientOperOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "482 : OPER : <channel> : :You're not channel operator\r\n"));
  if (!server.isClientOnChannel(providedClientToInvite, providedChannel))
    return(sendResponse(client, "443: OPER : <user> <channel> :is already on channel\r\n"));
  //RPL_INVITING : Returned by the server to indicate that the attempted INVITE message was successful and is being passed onto the end client : sendResponse(client, "341 : OPER" : "<channel> <nick>"\r\n");
}

void CommandHandler::handleOperMode(Client &client, const std::vector<std::string> &args, Server &server) {
  // if (args.size() < x) {
	// 	sendResponse(client, "ERR_NEEDMOREPARAMS :Not enough parameters\r\n");
	// 	return;
	// }
  std::string providedChannel = args[1];
  std::string providedModes = args[2];
  (void) client;
  (void) server;

  //check if client is OP in specified channel : sendResponse(client, "ERR_CHANOPRIVSNEEDED "<channel> : :You're not channel operator\r\n");
  //check if providedClientToKick is connected to channel: sendResponse(client, " 441 ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"\r\n");

}