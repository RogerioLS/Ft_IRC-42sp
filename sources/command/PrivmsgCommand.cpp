#include "../../includes/command/PrivmsgCommand.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include <string>

void PrivmsgCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    if (args.size() < 2) {
        debug.debugPrint("PRIVMSG command missing arguments", YELLOW);
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
        debug.debugPrint("Client " + client.getClientNickName() + " sending message to channel " + target, MAGENTA);
        Channel* channel = server.getChannelByName(target);
        if (channel) {
            const std::set<int>& clientIds = channel->getClientsById();
            for (std::set<int>::const_iterator it = clientIds.begin(); it != clientIds.end(); ++it) {
                if (*it != client.getClientId()) { // Não enviar para o próprio remetente
                    Client* destClient = server.getClientById(*it);
                    if (destClient) {
                        server.sendMessage(destClient->getClientFd(), fullMessage);
                    }
                }
            }
        } else {
            debug.debugPrint("Channel " + target + " not found", YELLOW);
            // Enviar ERR_NOSUCHCHANNEL
        }
    } else {
        // Enviar para usuário
        debug.debugPrint("Client " + client.getClientNickName() + " sending message to user " + target, MAGENTA);
        Client* destClient = server.getClientByNickname(target);
        if (destClient) {
            server.sendMessage(destClient->getClientFd(), fullMessage);
        } else {
            debug.debugPrint("User " + target + " not found", YELLOW);
            // Enviar ERR_NOSUCHNICK
        }
    }
}
