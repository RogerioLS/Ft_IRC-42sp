/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:11 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 12:40:04 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"

void JoinCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
    debug.debugPrint("[JOIN] Handling JOIN command", BLUE);

    if (!client.isFullyRegistered()) {
        debug.debugPrint("[JOIN] Client not fully registered", YELLOW);
        return (server.sendMessage(client.getClientFd(), Messages::ERR_NOTREGISTERED(client.getClientNickName())));
    }

    if (args.empty()) {
        debug.debugPrint("[JOIN] Missing arguments", YELLOW);
        return (server.sendMessage(client.getClientFd(), Messages::ERR_NEEDMOREPARAMS(client.getClientNickName(), "JOIN")));
    }

    const std::string& channelName = args[0];
    debug.debugPrint("[JOIN] Channel name: " + channelName, BLUE);

    if (channelName[0] != '#') {
        debug.debugPrint("[JOIN] Invalid channel name: " + channelName, YELLOW);
        return (server.sendMessage(client.getClientFd(), Messages::ERR_BADCHANMASK(client.getClientNickName(), channelName, "JOIN")));
    }

    Channel* channel = server.getChannelByName(channelName);

    if (channel) {
        if (channel->isClientInChannel(client.getClientId())) {
            debug.debugPrint("[JOIN] Client already in channel", YELLOW);
            return;
        }

        if (channel->getUserLimit() > 0 && (int)channel->getClientsById().size() >= channel->getUserLimit()) {
            debug.debugPrint("[JOIN] Channel is full", YELLOW);
            return (server.sendMessage(client.getClientFd(), Messages::ERR_CHANNELISFULL(client.getClientNickName(), channelName)));
        }

        if (channel->getInviteOnly()) {
            const std::set<int>& invited = channel->getInvitedById();
            if (invited.find(client.getClientId()) == invited.end()) {
                debug.debugPrint("[JOIN] Client not invited to channel", YELLOW);
                return (server.sendMessage(client.getClientFd(), Messages::ERR_INVITEONLYCHAN(client.getClientNickName(), channelName)));
            }
        }

        if (!channel->getKey().empty()) {
            if (args.size() < 2 || args[1] != channel->getKey()) {
                debug.debugPrint("[JOIN] Invalid key for channel", YELLOW);
                return (server.sendMessage(client.getClientFd(), Messages::ERR_BADCHANNELKEY(client.getClientNickName(), channelName)));
            }
        }

        channel->setClientsById(client.getClientId());
        debug.debugPrint("[JOIN] Client " + client.getClientNickName() + " joined channel " + channelName, MAGENTA);
    } else {
        server.createChannel(channelName, client);
        channel = server.getChannelByName(channelName);
        debug.debugPrint("[JOIN] Channel " + channelName + " created and client " + client.getClientNickName() + " joined", MAGENTA);
    }

    std::string joinMsg = ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " JOIN " + channelName + "\r\n";
    server.sendMessage(client.getClientFd(), joinMsg);
    channel->broadcastToAll(server, joinMsg);

    if (!channel->getTopic().empty()) {
        server.sendMessage(client.getClientFd(), Messages::RPL_TOPIC(client.getClientNickName(), channelName, channel->getTopic()));
    }

    std::string usersList;
    const std::set<int>& clients = channel->getClientsById();
    for (std::set<int>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        const Client* member = server.getClientInstFromId(*it);
        if (member) {
            usersList += (channel->getOperatorsById().count(member->getClientId()) ? "@" : "") + member->getClientNickName() + " ";
        }
    }

    server.sendMessage(client.getClientFd(), Messages::RPL_NAMREPLY(client.getClientNickName(), channelName, usersList));
    server.sendMessage(client.getClientFd(), Messages::RPL_ENDOFNAMES(client.getClientNickName(), channelName));

    debug.debugPrint("[JOIN] Finished handling JOIN command", BLUE);
}
