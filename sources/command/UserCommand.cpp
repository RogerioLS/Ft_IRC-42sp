#include "../../includes/command/UserCommand.hpp"
#include "../../includes/utils/Messages.hpp"
#include "../../includes/utils/Colors.hpp"

void UserCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    if (!client.hasValidPass() || !client.hasValidNick()) {
        server.sendMessage(client.getClientFd(), ":server NOTICE * :You must provide a password and nickname first.\r\n");
        debug.debugPrint("[USER] Client has no valid pass or nick.", YELLOW);
        return;
    }

    if (client.isFullyRegistered()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_ALREADYREGISTRED(client.getClientNickName()));
        debug.debugPrint("[USER] Client already registered.", YELLOW);
        return;
    }

    if (args.size() < 4) {
        server.sendMessage(client.getClientFd(), Messages::ERR_NEEDMOREPARAMS(client.getClientNickName(), "USER"));
        debug.debugPrint("[USER] Not enough parameters.", YELLOW);
        return;
    }

    client.setClientUserName(args[0]);
    // args[1] is hostname, often ignored.
    // args[2] is servername, often ignored.
    client.setClientRealName(args[3]);
    client.setHasValidUser(true);
    debug.debugPrint("[USER] User details set.", GREEN);

    if (client.isFullyRegistered()) {
        server.sendMessage(client.getClientFd(), Messages::RPL_WELCOME(client.getClientNickName()));
        debug.debugPrint("[USER] Client is now fully registered.", GREEN);
    }
}
