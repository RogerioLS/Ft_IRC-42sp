/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:52:49 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/08/04 20:05:35 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"
#include "../includes/server/Channel.hpp"
#include "../includes/parser/Parser.hpp"

namespace {

  bool handleInviteMode(Server &server, Client &client, Channel &channel, char flag) {
    if (flag == '-' && channel.getInviteOnly() == true) {
      channel.setInviteOnly(false);
      server.getDebug().debugPrint("[MODE] Invite-only mode removed from " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " -i\r\n");
      return true;
    }
    else if (flag == '+' && channel.getInviteOnly() == false) {
      channel.setInviteOnly(true);
      server.getDebug().debugPrint("[MODE] Invite-only mode set on " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " +i\r\n");
      return true;
    }
    return false;
  }

  bool handleTopicMode(Server &server, Client &client, Channel &channel, char flag) {

    if (flag == '-' && channel.getRestrictTopic() == true) {
      channel.setRestrictTopic(false);
      server.getDebug().debugPrint("[MODE] Topic restriction removed from " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " -t\r\n");
      return true;
    }
    else if (flag == '+' && channel.getRestrictTopic() == false) {
      channel.setRestrictTopic(true);
      server.getDebug().debugPrint("[MODE] Topic restriction set on " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " +t\r\n");
      return true;
    }
    return false;
  }

  bool handleKeyMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {
    if (arg.empty()) {
      server.getDebug().debugPrint("[MODE] Key mode argument missing", YELLOW);
      return (server.sendMessage(client.getClientFd(), channel.getName() + "o * :You must specify a parameter for the key mode. Syntax: <key>\r\n"), false);
    }

    std::string providedKey = arg;
    if (flag == '-' && channel.getKey() == providedKey) {
      channel.setKey("");
      server.getDebug().debugPrint("[MODE] Channel key removed from " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " -k\r\n");
      return true;
    }
    else if (flag == '-' && channel.getKey() != providedKey) {
      server.getDebug().debugPrint("[MODE] Channel key mismatch for removal on " + channel.getName(), YELLOW);
      return (server.sendMessage(client.getClientFd(), channel.getName() + " :Channel key already set" + "\r\n"), false);
    }
    else if (flag == '+' && channel.getKey().length() == 0) {
      channel.setKey(providedKey);
      server.getDebug().debugPrint("[MODE] Channel key set on " + channel.getName() + " to " + providedKey, GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " +k\r\n");
      return true;
    }
    server.getDebug().debugPrint("[MODE] No change to channel key for " + channel.getName(), YELLOW);
    return false;
  }

  bool handleLimitMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {
    server.getDebug().debugPrint("[MODE] handleLimitMode: flag=" + std::string(1, flag) + " channel=" + channel.getName() + " arg=" + arg, CYAN);
    if (flag == '-' && channel.getUserLimit() > 0) {
      channel.setUserLimit(0);
      server.getDebug().debugPrint("[MODE] User limit removed from " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " -l\r\n");
      return true;
    }
    else if (flag == '+') {
      std::string numericArg;

      if (!arg.empty() && std::isdigit(arg[0])) {
        for (std::string::const_iterator it = arg.begin(); it != arg.end() && std::isdigit(*it); ++it)
          numericArg.push_back(*it);

        if (numericArg.length() > 10 || numericArg.empty()) {
          server.getDebug().debugPrint("[MODE] Invalid limit mode parameter: " + arg, YELLOW);
          return (server.sendMessage(client.getClientFd(), channel.getName() + " l " + arg + " :Invalid limit mode parameter. Syntax: <limit>.\r\n"), false);
        }

        std::stringstream ss(numericArg);
        int limit;
        ss >> limit;

        if (ss.fail()) {
          server.getDebug().debugPrint("[MODE] Failed to parse limit: " + numericArg, YELLOW);
          return (server.sendMessage(client.getClientFd(), channel.getName() + " l " + arg + " :Invalid limit mode parameter. Syntax: <limit>.\r\n"), false);
        }

        if (limit > 0 && limit != channel.getUserLimit()) {
          channel.setUserLimit(limit);
          server.getDebug().debugPrint("[MODE] User limit set on " + channel.getName() + " to " + numericArg, GREEN);
          channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " +l\r\n");
          return true;
        }
      }
      else if (arg.empty()) {
        server.getDebug().debugPrint("[MODE] Limit mode argument missing", YELLOW);
        return (server.sendMessage(client.getClientFd(), channel.getName() + " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\r\n"), false);
      }
    }
    return false;
  }

  bool handleOperatorMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {
    std::string providedChannel  = channel.getName();
    std::string clientNick = client.getClientNickName();
    std::string clientToGiveOper = arg;
    int clientFd = client.getClientFd();

    server.getDebug().debugPrint("[MODE] handleOperatorMode: flag=" + std::string(1, flag) + " channel=" + channel.getName() + " arg=" + clientToGiveOper, CYAN);

    if (arg.empty()) {
      server.getDebug().debugPrint("[MODE] Operator mode argument missing", YELLOW);
      return (server.sendMessage(clientFd, channel.getName() + " * :You must specify a parameter for the op mode. Syntax: <nick>\r\n"), false);
    }

    if (!server.isClientFullyRegistered(clientToGiveOper)) {
      server.getDebug().debugPrint("[MODE] Client to give operator nick not fully registered: " + clientToGiveOper, YELLOW);
      return (server.sendMessage(clientFd, Messages::ERR_NOSUCHNICK(client.getClientNickName(), clientToGiveOper)), false);
    }

    if (!server.isClientOnChannel(clientToGiveOper, providedChannel)) {
      server.getDebug().debugPrint("[MODE] Target nick on channel: " + providedChannel, YELLOW);
      return (server.sendMessage(clientFd, Messages::ERR_NOTONCHANNEL(clientNick, providedChannel)), false);
    }

    int clientIdToOper = server.getClientIdFromNickname(clientToGiveOper);
    std::set<int> channelOpers = channel.getOperatorsById();
    bool isClientOper = (channelOpers.find(clientIdToOper) != channelOpers.end());

    if (isClientOper && flag == '-') {
      channel.removeOper(clientIdToOper);
      server.getDebug().debugPrint("[MODE] Operator status removed from " + clientToGiveOper + " on " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " -o " + clientToGiveOper + "\r\n");
      return true;
    }

    else if (!isClientOper && flag == '+' && clientIdToOper != client.getClientId()) {
      channel.addOper(clientIdToOper);
      server.getDebug().debugPrint("[MODE] Operator status given to " + clientToGiveOper + " on " + channel.getName(), GREEN);
      channel.broadcastToAll(server, ":" + client.getClientNickName() + "!" + client.getClientUserName() + "@" + client.getClientipAddress() + " MODE " + channel.getName() + " +o " + clientToGiveOper + "\r\n");
      return true;
    }
    server.getDebug().debugPrint("[MODE] No change to operator status for " + clientToGiveOper + " on " + channel.getName(), YELLOW);
    return false;
  }

  bool handleSingleMode(Server &server, Client &client, const std::string &providedChannel, char flag, char mode, const std::string &arg) {
    server.getDebug().debugPrint("[MODE] handleSingleMode: channel=" + providedChannel + " flag=" + std::string(1, flag) + " mode=" + std::string(1, mode) + " arg=" + arg, CYAN);
    Channel* channel = server.getChannelByName(providedChannel);

    if (!channel) {
      server.getDebug().debugPrint("[MODE] Channel not found: " + providedChannel, RED);
      return false;
    }

    switch (mode) {
      case 'i': return handleInviteMode(server, client, *channel, flag);
      case 't': return handleTopicMode(server, client, *channel, flag);
      case 'k': return handleKeyMode(server, client, *channel, flag, arg);
      case 'l': return handleLimitMode(server, client, *channel, flag, arg);
      case 'o': return handleOperatorMode(server, client, *channel, flag, arg);
      default:
        server.getDebug().debugPrint("[MODE] Unknown mode: " + std::string(1, mode), YELLOW);
        return false;
    }
  }

  bool handleChannelMode(Server &server, Client &client, const std::string &providedChannel, const std::string &providedModes, const std::vector<std::string> & providedModesArgs) {
    server.getDebug().debugPrint("[MODE] handleChannelMode: channel=" + providedChannel + " modes=" + providedModes, CYAN);
    bool atLeastOneModeSucceeded = false;
    size_t argIndex = 0;

    for (size_t i = 0; i < providedModes.length(); i += 2) {
      char flag = providedModes[i];
      char mode = providedModes[i + 1];
      std::string arg = (mode == 'k' || mode == 'l' || mode == 'o') && argIndex < providedModesArgs.size() ? providedModesArgs[argIndex] : "";

      if (handleSingleMode(server, client, providedChannel, flag, mode, arg))
        atLeastOneModeSucceeded = true;

      if (mode == 'k' || (mode == 'l' && flag == '+' ) || mode == 'o')
        argIndex++;
    }

    if (!atLeastOneModeSucceeded)
      server.getDebug().debugPrint("[MODE] No mode changes succeeded for channel: " + providedChannel, YELLOW);
    else
      server.getDebug().debugPrint("[MODE] At least one mode change succeeded for channel: " + providedChannel, GREEN);

  return atLeastOneModeSucceeded;
  }

  bool displayChannelModes(Server & server, Client & client, std::string providedChannel) {
    const Channel * channel = server.getChannelByName(providedChannel);

    std::string modeRules = "";
    if (!channel) {
      modeRules = "No modes to set";
      server.getDebug().debugPrint("[MODE] Channel Modes are: " + modeRules, YELLOW);
      return(server.sendMessage(client.getClientFd(), Messages::RPL_CHANNELMODEIS(providedChannel, modeRules)), false);
    }

    std::string modes = providedChannel + " " + channel->getTopic() + " " + channel->getKey();
    if (channel->getInviteOnly())
      modeRules += "i";
    if (channel->getRestrictTopic())
      modeRules += "t";
    if (channel->getUserLimit() > 0)
      modeRules += "l";
    if (!channel->getKey().empty())
      modeRules += "k";
    if (server.isClientOperOnChannel(client.getClientNickName(), providedChannel))
      modeRules += "o";

    if (modeRules.empty())
      modeRules = "No modes to set";
    else
      modeRules = "+" + modeRules;

    server.getDebug().debugPrint("[MODE] Channel Modes are: " + modeRules, YELLOW);
    return(server.sendMessage(client.getClientFd(), Messages::RPL_CHANNELMODEIS(channel->getName(), modeRules)), true);
  }
}

void ModeCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  std::string clientNick = client.getClientNickName();
  int clientFd = client.getClientFd();

  if (args.empty()) {
    debug.debugPrint("[MODE] Need channel params", YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NEEDMOREPARAMS(clientNick, "MODE")));
  }

  std::string providedChannel  = args[0];

  if (!client.isFullyRegistered()) {
    debug.debugPrint("[MODE] Client not registered: " + clientNick, YELLOW);
    server.sendMessage(clientFd, Messages::ERR_NOTREGISTERED(clientNick));
  }

  if (!server.isChannelRegistered(providedChannel)) {
    debug.debugPrint("[MODE] Channel not registered: " + providedChannel, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHCHANNEL(clientNick, providedChannel)));
  }

  if (!server.isClientOnChannel(clientNick, providedChannel)) {
    debug.debugPrint("[MODE] Client not on channel: " + providedChannel, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NOTONCHANNEL(clientNick, providedChannel)));
  }

  if (args.size() == 1) {
    if (displayChannelModes(server, client, providedChannel))
      debug.debugPrint("[MODE] Display Channel Modes successufully " , GREEN);
    else
      debug.debugPrint("[MODE] Display Channel Modes error " , YELLOW);
    return;
  }

  if (args.size() < 2) {
    debug.debugPrint("[MODE] Not enough parameters", YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_NEEDMOREPARAMS(clientNick, "MODE")));
  }

  std::string providedModes = Parser::formatOperatorModes(args[1]);
  std::vector<std::string> providedModesArgs = Parser::formatOperatorModeArgs(args);

  debug.debugPrint("[MODE] Command received from: " + clientNick, CYAN);
  debug.debugPrint("[MODE] Modes: " + providedModes, GREEN);
  for (size_t i = 0; i < providedModesArgs.size(); ++i) {
    std::stringstream ss;
    ss << i;
    debug.debugPrint("[MODE] Mode arg[" + ss.str() + "]: " + providedModesArgs[i], GREEN);
  }

  if (!server.isClientOperOnChannel(clientNick, providedChannel)) {
    debug.debugPrint("[MODE] Client is not channel operator: " + clientNick, YELLOW);
    return(server.sendMessage(clientFd, Messages::ERR_CHANOPRIVSNEEDED(clientNick, providedChannel)));
  }

  if (!handleChannelMode(server, client ,providedChannel, providedModes, providedModesArgs)) {
    debug.debugPrint("[MODE] Error while setting mode to channel: " + providedChannel, RED);
    return(server.sendMessage(clientFd, "Error while setting mode to channel\r\n"));
  }

  debug.debugPrint("[MODE] Mode(s) set successfully on channel: " + providedChannel, GREEN);
}
