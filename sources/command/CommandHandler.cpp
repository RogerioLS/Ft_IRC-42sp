#include "../../includes/command/CommandHandler.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include <string>

CommandHandler::CommandHandler(Server& server, Debug& debug) : _server(server), _debug(debug) {
    _populateCommands();
    _debug.debugPrint("✅ CommandHandler initialized", GREEN);
}

CommandHandler::~CommandHandler() {}

void CommandHandler::_populateCommands() {
    _commands["JOIN"] = &CommandHandler::_handleJoin;
    _commands["PRIVMSG"] = &CommandHandler::_handlePrivmsg;
    _commands["TOPIC"] = &CommandHandler::_handleTopic;
    _commands["HELP"] = &CommandHandler::_handleHelp;
    _debug.debugPrint("✅ Commands populated", GREEN);
}

void CommandHandler::executeCommand(Client& client, const std::string& message) {
    std::stringstream ss(message);
    std::string line;

    while (std::getline(ss, line)) {
        // Remove \r from the end of the line
        if (!line.empty() && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }

        if (line.empty()) {
            continue;
        }

        _debug.debugPrint("Executing for client " + utils::intToString(client.getClientId()) + ": " + line, CYAN);

        std::stringstream line_ss(line);
        std::string command;
        line_ss >> command;

        std::vector<std::string> args;
        std::string arg;
        while (line_ss >> arg) {
            args.push_back(arg);
        }

        std::map<std::string, CommandFunction>::iterator it = _commands.find(command);
        if (it != _commands.end()) {
            (this->*(it->second))(client, args);
        } else {
            _debug.debugPrint("Unknown command: " + command, RED);
            // Aqui enviaremos uma resposta de erro para o cliente, como ERR_UNKNOWNCOMMAND
        }
    }
}

void CommandHandler::_handleJoin(Client& client, const std::vector<std::string>& args) {
    if (args.empty()) {
        _debug.debugPrint("JOIN command missing arguments", YELLOW);
        // Enviar ERR_NEEDMOREPARAMS para o cliente
        return;
    }

    const std::string& channelName = args[0];

    // Validação simples do nome do canal
    if (channelName[0] != '#') {
        _debug.debugPrint("Invalid channel name: " + channelName, YELLOW);
        // Enviar ERR_BADCHANMASK ou similar
        return;
    }

    _debug.debugPrint("Client " + client.getClientNickName() + " wants to join " + channelName, MAGENTA);

    Channel* channel = _server.getChannelByName(channelName);
    if (channel) {
        // Canal existe, adicionar cliente
        _debug.debugPrint("Channel " + channelName + " already exists. Adding client.", CYAN);
        channel->setClientsById(client.getClientId());
    } else {
        // Canal não existe, criar
        _debug.debugPrint("Channel " + channelName + " does not exist. Creating.", CYAN);
        _server.createChannel(channelName, client);
    }

    // TODO: Enviar respostas IRC (RPL_TOPIC, RPL_NAMREPLY, etc.)
}

void CommandHandler::_handlePrivmsg(Client& client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        _debug.debugPrint("PRIVMSG command missing arguments", YELLOW);
        // Enviar ERR_NEEDMOREPARAMS
        return;
    }

    const std::string& target = args[0];
    std::string message;
    for (size_t i = 1; i < args.size(); ++i) {
        if (i > 1) message += " ";
        message += args[i];
    }

    if (message[0] == ':') {
        message = message.substr(1);
    }

    std::string fullMessage = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientHostName() + " PRIVMSG " + target + " :" + message + "\r\n";

    if (target[0] == '#') {
        // Enviar para canal
        _debug.debugPrint("Client " + client.getClientNickName() + " sending message to channel " + target, MAGENTA);
        Channel* channel = _server.getChannelByName(target);
        if (channel) {
            const std::set<int>& clientIds = channel->getClientsById();
            for (std::set<int>::const_iterator it = clientIds.begin(); it != clientIds.end(); ++it) {
                if (*it != client.getClientId()) { // Não enviar para o próprio remetente
                    Client* destClient = _server.getClientById(*it);
                    if (destClient) {
                        send(destClient->getClientFd(), fullMessage.c_str(), fullMessage.length(), 0);
                    }
                }
            }
        } else {
            _debug.debugPrint("Channel " + target + " not found", YELLOW);
            // Enviar ERR_NOSUCHCHANNEL
        }
    } else {
        // Enviar para usuário
        _debug.debugPrint("Client " + client.getClientNickName() + " sending message to user " + target, MAGENTA);
        Client* destClient = _server.getClientByNickname(target);
        if (destClient) {
            send(destClient->getClientFd(), fullMessage.c_str(), fullMessage.length(), 0);
        } else {
            _debug.debugPrint("User " + target + " not found", YELLOW);
            // Enviar ERR_NOSUCHNICK
        }
    }
}

void CommandHandler::_handleTopic(Client& client, const std::vector<std::string>& args) {
    if (args.empty()) {
        _debug.debugPrint("TOPIC command missing arguments", YELLOW);
        // Enviar ERR_NEEDMOREPARAMS
        return;
    }

    const std::string& channelName = args[0];
    Channel* channel = _server.getChannelByName(channelName);

    if (!channel) {
        _debug.debugPrint("Channel " + channelName + " not found", YELLOW);
        // Enviar ERR_NOSUCHCHANNEL
        return;
    }

    if (args.size() == 1) {
        // Ver o tópico
        const std::string& topic = channel->getTopic();
        std::string topic_message;
        if (topic.empty()) {
            topic_message = ":" + _server.getServerName() + " 331 " + client.getClientNickName() + " " + channelName + " :No topic is set\r\n";
            _debug.debugPrint("No topic set for channel " + channelName, CYAN);
        } else {
            topic_message = ":" + _server.getServerName() + " 332 " + client.getClientNickName() + " " + channelName + " :" + topic + "\r\n";
            _debug.debugPrint("Topic for channel " + channelName + " is: " + topic, CYAN);
        }
        send(client.getClientFd(), topic_message.c_str(), topic_message.length(), 0);
    } else {
        // Definir o tópico
        const std::set<int>& operators = channel->getOperatorsById();
        if (channel->getRestrictTopic() && operators.find(client.getClientId()) == operators.end()) {
            _debug.debugPrint("Client " + client.getClientNickName() + " is not an operator of " + channelName, YELLOW);
            // Enviar ERR_CHANOPRIVSNEEDED
            return;
        }

        std::string newTopic;
        for (size_t i = 1; i < args.size(); ++i) {
            if (i > 1) newTopic += " ";
            newTopic += args[i];
        }
        if (newTopic[0] == ':') {
            newTopic = newTopic.substr(1);
        }

        channel->setTopic(newTopic);
        _debug.debugPrint("Client " + client.getClientNickName() + " set topic for " + channelName + " to: " + newTopic, MAGENTA);

        std::string topic_msg_broadcast = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientHostName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
        const std::set<int>& clientIds = channel->getClientsById();
        for (std::set<int>::const_iterator it = clientIds.begin(); it != clientIds.end(); ++it) {
            Client* destClient = _server.getClientById(*it);
            if (destClient) {
                send(destClient->getClientFd(), topic_msg_broadcast.c_str(), topic_msg_broadcast.length(), 0);
            }
        }
    }
}

void CommandHandler::_handleHelp(Client& client, const std::vector<std::string>& args) {
    (void)args;
    _debug.debugPrint("Executing HELP command for client " + client.getClientNickName(), MAGENTA);

    std::string serverName = _server.getServerName();
    std::string nickName = client.getClientNickName();

    std::string help_message = ":" + serverName + " NOTICE " + nickName + " :Available commands:\r\n";
    send(client.getClientFd(), help_message.c_str(), help_message.length(), 0);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  JOIN <#channel> - Join or create a channel.\r\n";
    send(client.getClientFd(), help_message.c_str(), help_message.length(), 0);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  PRIVMSG <target> :<message> - Send a private message to a user or channel.\r\n";
    send(client.getClientFd(), help_message.c_str(), help_message.length(), 0);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel.\r\n";
    send(client.getClientFd(), help_message.c_str(), help_message.length(), 0);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  HELP - Shows this help message.\r\n";
    send(client.getClientFd(), help_message.c_str(), help_message.length(), 0);
}