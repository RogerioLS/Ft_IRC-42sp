#include "../../includes/command/PassCommand.hpp"
#include "../../includes/utils/Messages.hpp"
#include "../../includes/utils/Colors.hpp"

void PassCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    if (client.isFullyRegistered()) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_ALREADYREGISTRED, client.getClientNickName(), ":You may not reregister"));
        debug.debugPrint("[PASS] Client already registered.", YELLOW);
        return;
    }

    if (args.empty()) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_NEEDMOREPARAMS, client.getClientNickName(), "PASS :Not enough parameters"));
        debug.debugPrint("[PASS] Missing password argument.", YELLOW);
        return;
    }

    const std::string& password = args[0];
    if (password == server.getPassword()) {
        client.setHasValidPass(true);
        debug.debugPrint("[PASS] Client authenticated successfully.", GREEN);
    } else {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_PASSWDMISMATCH, client.getClientNickName(), ":Password incorrect"));
        debug.debugPrint("[PASS] Incorrect password attempt.", YELLOW);
    }
}
