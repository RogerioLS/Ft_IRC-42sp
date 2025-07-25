#include "../../includes/command/CommandHandler.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include "../../includes/command/JoinCommand.hpp"
#include "../../includes/command/PrivmsgCommand.hpp"
#include "../../includes/command/TopicCommand.hpp"
#include "../../includes/command/HelpCommand.hpp"
#include "../../includes/command/PassCommand.hpp"
#include "../../includes/command/NickCommand.hpp"
#include "../../includes/command/UserCommand.hpp"
#include <string>
#include <sstream> // Adicionado para std::stringstream

CommandHandler::CommandHandler(IServer& server, Debug& debug) : _server(server), _debug(debug) {
    _populateCommands();
    _debug.debugPrint("✅ CommandHandler initialized", GREEN);
}

CommandHandler::~CommandHandler() {}

void CommandHandler::_populateCommands() {
    _commands["PASS"] = &PassCommand::execute;
    _commands["NICK"] = &NickCommand::execute;
    _commands["USER"] = &UserCommand::execute;
    _commands["JOIN"] = &JoinCommand::execute;
    _commands["PRIVMSG"] = &PrivmsgCommand::execute;
    _commands["TOPIC"] = &TopicCommand::execute;
    _commands["HELP"] = &HelpCommand::execute;
    _debug.debugPrint("✅ Commands populated", GREEN);
}

void CommandHandler::executeCommand(Client& client, const std::string& message) {
    std::string msg = message;

    // Trim \r\n from the end
    if (!msg.empty() && msg.length() > 0 && msg[msg.length() - 1] == '\n') {
        msg.erase(msg.length() - 1);
    }
    if (!msg.empty() && msg.length() > 0 && msg[msg.length() - 1] == '\r') {
        msg.erase(msg.length() - 1);
    }

    if (msg.empty()) {
        return;
    }

    _debug.debugPrint("[CMD] Executing line for client " + utils::intToString(client.getClientId()) + ": " + msg, CYAN);

    std::vector<std::string> params;
    std::string command;
    std::string trailing;
    
    // Find and split the trailing part of the message
    size_t trailing_start = msg.find(" :");
    if (trailing_start != std::string::npos) {
        trailing = msg.substr(trailing_start + 2);
        msg = msg.substr(0, trailing_start);
    }

    // Split the first part of the message (command and regular params)
    std::stringstream ss(msg);
    ss >> command;

    std::string arg;
    while (ss >> arg) {
        params.push_back(arg);
    }

    // Add the trailing param to the list of arguments
    if (!trailing.empty()) {
        params.push_back(trailing);
    }

    _debug.debugPrint("[CMD] Found command: " + command, BLUE);
    std::map<std::string, CommandFunction>::iterator it = _commands.find(command);
    if (it != _commands.end()) {
        _debug.debugPrint("[CMD] Executing handler for " + command, BLUE);
        (it->second)(_server, client, params, _debug);
        _debug.debugPrint("[CMD] Finished handler for " + command, BLUE);
    } else {
        _debug.debugPrint("[CMD] Unknown command: " + command, RED);
        // You might want to send ERR_UNKNOWNCOMMAND to the client here
    }
}