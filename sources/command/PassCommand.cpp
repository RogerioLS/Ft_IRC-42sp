#include "../../includes/command/PassCommand.hpp"
#include "../../includes/utils/Messages.hpp"
#include "../../includes/utils/Colors.hpp"

void PassCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    if (client.isFullyRegistered()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_ALREADYREGISTRED(client.getClientNickName()));
        debug.debugPrint("[PASS] Client already registered.", YELLOW);
        return;
    }

    if (args.empty()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_NEEDMOREPARAMS(client.getClientNickName(), "PASS"));
        debug.debugPrint("[PASS] Missing password argument.", YELLOW);
        return;
    }

    const std::string& password = args[0];
    if (password == server.getPassword()) {
        client.setHasValidPass(true);
        debug.debugPrint("[PASS] Client authenticated successfully.", GREEN);
    } else {
        server.sendMessage(client.getClientFd(), Messages::ERR_PASSWDMISMATCH(client.getClientNickName()));
        debug.debugPrint("[PASS] Incorrect password attempt.", YELLOW);
    }
}
