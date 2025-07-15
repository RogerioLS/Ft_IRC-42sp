/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:31 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 11:25:02 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "../utils/IRC.hpp"

class Client;

class Server;

class CommandHandler {

	private:
		CommandHandler();

	public:
		static void processCommand(Client &client, const std::string &command, class Server &server);
		static void handlePass(Client &client, const std::vector<std::string> &args, class Server &server);
		static void handleNick(Client &client, const std::vector<std::string> &args, class Server &server);
		static void handleUser(Client &client, const std::vector<std::string> &args);
    static void handleOperKick(Client &client, const std::vector<std::string> &args, Server &server);
    static void handleOperInvite(Client &client, const std::vector<std::string> &args, Server &server);
    static void handleOperMode(Client &client, const std::vector<std::string> &args, Server &server);
		static void sendResponse(Client &client, const std::string &response);
		static bool isValidNickname(const std::string &nickname);
		static bool isNicknameInUse(const std::string &nickname, class Server &server);
    static bool inviteClientToChannel(Server &server, const std::string & providedChannel, const std::string & providedClientToInvite);
};

#endif
