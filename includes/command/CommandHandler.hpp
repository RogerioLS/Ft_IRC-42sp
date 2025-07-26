#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "../utils/IRC.hpp"
#include "../utils/Debug.hpp"
#include "../server/IServer.hpp"
#include "./JoinCommand.hpp"
#include "./PrivmsgCommand.hpp"
#include "./TopicCommand.hpp"
#include "./HelpCommand.hpp"
#include <map>
#include <string>
#include <vector>

class Client;

class CommandHandler {
private:
    typedef void (*CommandFunction)(IServer&, Client&, const std::vector<std::string>&, Debug&);

    IServer& _server;
    Debug&  _debug;
    std::map<std::string, CommandFunction> _commands;

    void _populateCommands();

public:
    CommandHandler(IServer& server, Debug& debug);
    ~CommandHandler();

    void executeCommand(Client& client, const std::string& message);
};

#endif
