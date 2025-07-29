/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:07 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 12:29:23 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

CommandHandler::CommandHandler(Server& server, Debug& debug)
  : _server(server),
    _debug(debug)
  {
    populateCommands();
    _debug.debugPrint("✅ CommandHandler initialized", GREEN);
  }

CommandHandler::~CommandHandler() {}

void CommandHandler::populateCommands() {
  _commands["HELP"] = &HelpCommand::execute;
  _commands["INVITE"] = &InviteCommand::execute;
  _commands["JOIN"] = &JoinCommand::execute;
  _commands["KICK"] = &KickCommand::execute;
  _commands["MODE"] = &ModeCommand::execute;
  _commands["NICK"] = &NickCommand::execute;
  _commands["PASS"] = &PassCommand::execute;
  _commands["PRIVMSG"] = &PrivmsgCommand::execute;
  _commands["TOPIC"] = &TopicCommand::execute;
  _commands["USER"] = &UserCommand::execute;
  _debug.debugPrint("✅ Commands populated", GREEN);
}

void CommandHandler::executeCommand(Client& client, const std::string& message) {
  std::stringstream ss(message);
  std::string line;

  _debug.debugPrint("[CMD] Processing message: " + message, BLUE);

  while (std::getline(ss, line)) {
    // Remove \r from the end of the line
    if (!line.empty() && line[line.length() - 1] == '\r') {
      line.erase(line.length() - 1);
    }

    if (line.empty()) {
      _debug.debugPrint("[CMD] Skipping empty line.", BLUE);
      continue;
    }

    _debug.debugPrint("[CMD] Executing line for client " + Utils::intToString(client.getClientId()) + ": " + line, CYAN);

    std::stringstream line_ss(line);
    std::string command;
    line_ss >> command;

    std::vector<std::string> args;
    std::string arg;
    while (line_ss >> arg) {
      args.push_back(arg);
    }

    _debug.debugPrint("[CMD] Found command: " + command, BLUE);
    std::map<std::string, CommandFunction>::iterator it = _commands.find(command);
    if (it != _commands.end()) {
      _debug.debugPrint("[CMD] Executing handler for " + command, BLUE);
      (it->second)(_server, client, args, _debug);
      _debug.debugPrint("[CMD] Finished handler for " + command, BLUE);
    } else {
      _debug.debugPrint("[CMD] Unknown command: " + command, RED);
      _server.sendMessage(client.getClientFd(), "[CMD] Unknown command: " + command + RED);
    }
  }
  _debug.debugPrint("[CMD] Finished processing message.", BLUE);
}
