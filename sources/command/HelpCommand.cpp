#include "../../includes/command/HelpCommand.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include <string>

void HelpCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    (void)args;
    debug.debugPrint("Executing HELP command for client " + client.getClientNickName(), MAGENTA);

    std::string serverName = server.getServerName();
    std::string nickName = client.getClientNickName();

    std::string help_message = ":" + serverName + " NOTICE " + nickName + " :Available commands:\r\n";
    server.sendMessage(client.getClientFd(), help_message);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  JOIN <#channel> - Join or create a channel.\r\n";
    server.sendMessage(client.getClientFd(), help_message);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  PRIVMSG <target> :<message> - Send a private message to a user or channel.\r\n";
    server.sendMessage(client.getClientFd(), help_message);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel.\r\n";
    server.sendMessage(client.getClientFd(), help_message);

    help_message = ":" + serverName + " NOTICE " + nickName + " :  HELP - Shows this help message.\r\n";
    server.sendMessage(client.getClientFd(), help_message);
}
