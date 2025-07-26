#include "../../includes/command/CommandHandler.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include "../../includes/command/JoinCommand.hpp"
#include "../../includes/command/PrivmsgCommand.hpp"
#include "../../includes/command/TopicCommand.hpp"
#include "../../includes/command/HelpCommand.hpp"
#include <string>

CommandHandler::CommandHandler(IServer& server, Debug& debug) : _server(server), _debug(debug) {
    _populateCommands();
    _debug.debugPrint("✅ CommandHandler initialized", GREEN);
}

CommandHandler::~CommandHandler() {}

void CommandHandler::_populateCommands() {
    _commands["JOIN"] = &JoinCommand::execute;
    _commands["PRIVMSG"] = &PrivmsgCommand::execute;
    _commands["TOPIC"] = &TopicCommand::execute;
    _commands["HELP"] = &HelpCommand::execute;
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

        _debug.debugPrint("[CMD] Executing line for client " + utils::intToString(client.getClientId()) + ": " + line, CYAN);

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
            // Aqui enviaremos uma resposta de erro para o cliente, como ERR_UNKNOWNCOMMAND
        }
    }
    _debug.debugPrint("[CMD] Finished processing message.", BLUE);
}
