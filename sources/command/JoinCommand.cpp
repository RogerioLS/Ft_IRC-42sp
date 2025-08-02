#include "../../includes/command/JoinCommand.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include "../../includes/utils/Messages.hpp"

void JoinCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    (void)debug;
    if (args.empty()) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_NEEDMOREPARAMS, client.getClientNickName(), "JOIN :Not enough parameters"));
        return;
    }

    const std::string& channelName = args[0];

    if (channelName[0] != '#') {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_BADCHANMASK, client.getClientNickName(), channelName + " :Bad Channel Mask"));
        return;
    }

    Channel* channel = server.getChannelByName(channelName);
    if (!channel) {
        server.createChannel(channelName, client);
        channel = server.getChannelByName(channelName);
    } else {
        channel->setClientsById(client.getClientId());
    }

    // Broadcast JOIN message to all clients in the channel
    std::string joinMsg = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " JOIN " + channelName + "\r\n";
    channel->broadcastMessage(joinMsg, server);

    // Send topic
    const std::string& topic = channel->getTopic();
    if (topic.empty()) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), RPL_NOTOPIC, client.getClientNickName(), channelName + " :No topic is set"));
    } else {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), RPL_TOPIC, client.getClientNickName(), channelName + " :" + topic));
    }

    // Send names list
    std::string namesList;
    const std::set<int>& clientIds = channel->getClientsById();
    for (std::set<int>::const_iterator it = clientIds.begin(); it != clientIds.end(); ++it) {
        Client* member = server.getClientById(*it);
        if (member) {
            if (channel->isOperator(member->getClientId())) {
                namesList += "@";
            }
            namesList += member->getClientNickName() + " ";
        }
    }
    // Remove trailing space
    if (!namesList.empty()) {
        namesList.resize(namesList.size() - 1);
    }

    server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), RPL_NAMREPLY, client.getClientNickName(), "= " + channelName + " :" + namesList));
    server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), RPL_ENDOFNAMES, client.getClientNickName(), channelName + " :End of /NAMES list."));
}
