/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:31 by codespace         #+#    #+#             */
/*   Updated: 2025/07/22 12:13:43 by pmelo-ca         ###   ########.fr       */
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
    static bool executeOperKick(Client &client, const std::vector<std::string> &args, Server &server);
    static bool kickClientFromChannel(Server &server,  const std::string &providedChannel, const std::string &providedClientToKick, const std::string & providedOper, const std::string & reasonToKick);
    static bool executeOperInvite(Client &client, const std::vector<std::string> &args, Server &server);
    static bool inviteClientToChannel(Server &server, const std::string & providedChannel, const std::string & providedClientToInvite, const std::string & providedOper);
    static bool executeOperMode(Client &client, const std::vector<std::string> &args, Server &server);
    static bool handleOperMode(Server &server, const std::string & providedChannel, const std::string & providedModes, const std::vector<std::string> & providedModesArgs, const std::string & providedOper);
    static bool handleSingleMode(Server &server, const std::string &channel, char flag, char mode, const std::string &arg, const std::string &oper);
    static bool handleInviteMode(Server &server, const std::string &channel, bool enable);
    static bool handleTopicMode(Server &server, const std::string &channel, bool enable);
    static bool handleKeyMode(Server &server, const std::string &channel, bool enable, const std::string &arg);
    static bool handleLimitMode(Server &server, const std::string &channel, bool enable, const std::string &arg);
    static bool handleOperatorMode(Server &server, const std::string &channel, bool enable, const std::string &arg, const std::string &oper);
  };

#endif
