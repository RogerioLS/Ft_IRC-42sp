/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:31 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 17:22:35 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "../server/Client.hpp"
#include "../server/Server.hpp"

class CommandHandler {
	private:
		CommandHandler();
	public:
		static void processCommand(Client &client, const std::string &command, class Server &server);
		static void handlePass(Client &client, const std::vector<std::string> &args);
		static void handleNick(Client &client, const std::vector<std::string> &args, class Server &server);
		static void handleUser(Client &client, const std::vector<std::string> &args);
		static void sendResponse(Client &client, const std::string &response);
		static bool isValidNickname(const std::string &nickname);
		static bool isNicknameInUse(const std::string &nickname, class Server &server);
};

#endif

