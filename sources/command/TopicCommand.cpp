#include "../../includes/command/TopicCommand.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Colors.hpp"
#include "../../includes/utils/Utils.hpp"
#include "../../includes/utils/Messages.hpp"
#include <string>

void TopicCommand::execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    (void)debug;
    if (args.empty()) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_NEEDMOREPARAMS, client.getClientNickName(), "TOPIC :Not enough parameters"));
        return;
    }

    const std::string& channelName = args[0];
    Channel* channel = server.getChannelByName(channelName);

    if (!channel) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_NOSUCHCHANNEL, client.getClientNickName(), channelName + " :No such channel"));
        return;
    }

    // Check if client is in the channel
    if (!channel->isClientInChannel(client.getClientId())) {
        server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_NOTONCHANNEL, client.getClientNickName(), channelName + " :You're not on that channel"));
        return;
    }

    if (args.size() == 1) {
        // View topic
        const std::string& topic = channel->getTopic();
        if (topic.empty()) {
            server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), RPL_NOTOPIC, client.getClientNickName(), channelName + " :No topic is set"));
        } else {
            server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), RPL_TOPIC, client.getClientNickName(), channelName + " :" + topic));
        }
    } else {
        // Set topic
        if (channel->getRestrictTopic() && !channel->isOperator(client.getClientId())) {
            server.sendMessage(client.getClientFd(), irc::numericReply(server.getServerName(), ERR_CHANOPRIVSNEEDED, client.getClientNickName(), channelName + " :You're not channel operator"));
            return;
        }

        std::string newTopic;
        for (size_t i = 1; i < args.size(); ++i) {
            if (i > 1) newTopic += " ";
            newTopic += args[i];
        }
        if (!newTopic.empty() && newTopic[0] == ':') {
            newTopic = newTopic.substr(1);
        }

        channel->setTopic(newTopic);

        std::string topicMsgBroadcast = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
        channel->broadcastMessage(topicMsgBroadcast, server);
    }
}
