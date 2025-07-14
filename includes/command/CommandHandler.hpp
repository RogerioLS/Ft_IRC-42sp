#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "../utils/IRC.hpp"
#include "../utils/Debug.hpp"
#include <map>
#include <string>
#include <vector>

class Server;
class Client;

class CommandHandler {
private:
    typedef void (CommandHandler::*CommandFunction)(Client&, const std::vector<std::string>&);

    Server& _server;
    Debug&  _debug;
    std::map<std::string, CommandFunction> _commands;

    void _populateCommands();

    // Command handlers
    void _handleJoin(Client& client, const std::vector<std::string>& args);
    void _handlePrivmsg(Client& client, const std::vector<std::string>& args);
    void _handleTopic(Client& client, const std::vector<std::string>& args);
    void _handleHelp(Client& client, const std::vector<std::string>& args);

public:
    CommandHandler(Server& server, Debug& debug);
    ~CommandHandler();

    void executeCommand(Client& client, const std::string& message);
};

#endif