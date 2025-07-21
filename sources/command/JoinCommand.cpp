#include "../../includes/command/JoinCommand.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp" // Para utils::intToString

void JoinCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    debug.debugPrint("[JOIN] Handling JOIN command", BLUE);
    if (args.empty()) {
        debug.debugPrint("[JOIN] Missing arguments", YELLOW);
        // Enviar ERR_NEEDMOREPARAMS para o cliente
        return;
    }

    const std::string& channelName = args[0];
    debug.debugPrint("[JOIN] Channel name: " + channelName, BLUE);

    // Validação simples do nome do canal
    if (channelName[0] != '#') {
        debug.debugPrint("[JOIN] Invalid channel name: " + channelName, YELLOW);
        // Enviar ERR_BADCHANMASK ou similar
        return;
    }

    debug.debugPrint("[JOIN] Client " + client.getClientNickName() + " wants to join " + channelName, MAGENTA);

    Channel* channel = server.getChannelByName(channelName);
    if (channel) {
        // Canal existe, adicionar cliente
        debug.debugPrint("[JOIN] Channel " + channelName + " already exists. Adding client.", CYAN);
        channel->setClientsById(client.getClientId());
    } else {
        // Canal não existe, criar
        debug.debugPrint("[JOIN] Channel " + channelName + " does not exist. Creating.", CYAN);
        server.createChannel(channelName, client);
    }
    debug.debugPrint("[JOIN] Finished handling JOIN command", BLUE);
}
