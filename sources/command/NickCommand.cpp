#include "../../includes/command/NickCommand.hpp"
#include "../../includes/utils/Messages.hpp"
#include "../../includes/utils/Colors.hpp"

static bool isValidNickname(const std::string& nickname) {
    if (nickname.length() > 9) {
        return false;
    }
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!std::isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    return true;
}

void NickCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    if (!client.hasValidPass()) {
        server.sendMessage(client.getClientFd(), ":server NOTICE * :You must provide a password first.\r\n");
        debug.debugPrint("[NICK-TEST] Failing because client has no valid pass.", RED);
        return;
    }

    if (args.empty()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_NONICKNAMEGIVEN(client.getClientNickName()));
        debug.debugPrint("[NICK-TEST] Failing because args is empty.", RED);
        return;
    }

    const std::string& newNickname = args[0];

    if (!isValidNickname(newNickname)) {
        server.sendMessage(client.getClientFd(), Messages::ERR_ERRONEUSNICKNAME(client.getClientNickName(), newNickname));
        debug.debugPrint("[NICK-TEST] Failing because nickname is invalid.", RED);
        return;
    }

    Client* existingClient = server.getClientByNickname(newNickname);
    if (existingClient != NULL && existingClient->getClientId() != client.getClientId()) {
        server.sendMessage(client.getClientFd(), Messages::ERR_NICKNAMEINUSE(client.getClientNickName(), newNickname));
        debug.debugPrint("[NICK-TEST] Failing because nickname is in use.", RED);
        return;
    }

    client.setClientNickName(newNickname);
    client.setHasValidNick(true);
    debug.debugPrint("[NICK-TEST] Nickname set successfully to: " + newNickname, GREEN);
}
