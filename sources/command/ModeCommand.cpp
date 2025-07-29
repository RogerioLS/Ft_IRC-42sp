/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:52:49 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 12:25:49 by pmelo-ca         ###   ########.fr       */
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
      return (server.sendMessage(client.getClientFd(), client.getClientNickName() + " sets mode " + flag + "i on " + channel.getName() + "\r\n"), true);
    }
    else if (flag == '+' && channel.getInviteOnly() == false) {
      channel.setInviteOnly(true);
      return (server.sendMessage(client.getClientFd(), client.getClientNickName() + " sets mode " + flag + "i on " + channel.getName() + "\r\n"), true);
    }
    return false;
  }

  bool handleTopicMode(Server &server, Client &client, Channel &channel, char flag) {

    if (flag == '-' && channel.getRestrictTopic() == true) {
      channel.setRestrictTopic(false);
      channel.broadcastToAll(server, client.getClientNickName() + " sets mode " + flag + "t on " + channel.getName() + "\r\n");
      return true;
    }
    else if (flag == '+' && channel.getRestrictTopic() == false) {
      channel.setRestrictTopic(true);
      channel.broadcastToAll(server, client.getClientNickName() + " sets mode " + flag + "t on " + channel.getName() + "\r\n");
      return true;
    }
    return false;
  }

  bool handleKeyMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {
    if (arg.empty())
      return (server.sendMessage(client.getClientFd(), channel.getName() + "o * :You must specify a parameter for the key mode. Syntax: <nick>\r\n"), false);

    if (flag == '-' && channel.getKey() == arg) {
      channel.setKey("");
      channel.broadcastToAll(server, client.getClientNickName() + " removes channel keyword" + "\r\n");
      return true;
    }
    else if (flag == '-' && channel.getKey() != arg)
      return (server.sendMessage(client.getClientFd(), channel.getName() + " :Channel key already set" + "\r\n"), false);
    else if (flag == '+' && channel.getKey().length() == 0) {
      channel.setKey(arg);
      channel.broadcastToAll(server, client.getClientNickName() + " sets channel keyword to " +arg + "\r\n");
      return true;
    }
    return false;
  }

  bool handleLimitMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {
    if (flag == '-' && channel.getUserLimit() > 0) {
      channel.setUserLimit(0);
      return (server.sendMessage(client.getClientFd(), " removes user limit.\r\n"), true);
    }
    else if (flag == '+') {
      std::string numericArg;

      if (!arg.empty() && std::isdigit(arg[0])) {
        for (std::string::const_iterator it = arg.begin(); it != arg.end() && std::isdigit(*it); ++it)
          numericArg.push_back(*it);

        if (numericArg.length() > 10 || numericArg.empty())
          return (server.sendMessage(client.getClientFd(), channel.getName() + " l " + arg + " :Invalid limit mode parameter. Syntax: <limit>.\r\n"), false);

        std::stringstream ss(numericArg);
        int limit;
        ss >> limit;

        if (ss.fail())
          return (server.sendMessage(client.getClientFd(), channel.getName() + " l " + arg + " :Invalid limit mode parameter. Syntax: <limit>.\r\n"), false);

        if (limit > 0 && limit != channel.getUserLimit()) {
          channel.setUserLimit(limit);
          channel.broadcastToAll(server, client.getClientNickName() + " sets channel limit to " + numericArg + "\r\n");
          return true;
        }
      }
      else if (arg.empty())
        return (server.sendMessage(client.getClientFd(), channel.getName() + " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\r\n"), false);
    }
    return false;
  }

  bool handleOperatorMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {

    if (arg.empty())
      return (server.sendMessage(client.getClientFd(), channel.getName() + "o * :You must specify a parameter for the op mode. Syntax: <nick>\r\n"), false);

    if (!server.isClientRegistered(arg))
      return (server.sendMessage(client.getClientFd(), Messages::ERR_NOSUCHNICK(client.getClientNickName(), arg)), false);

    int clientIdToOper = server.getClientIdFromNickname(arg);
    std::set<int> channelOpers = channel.getOperatorsById();
    bool isClientOper = (channelOpers.find(clientIdToOper) != channelOpers.end());

    if (isClientOper && flag == '-') {
      channel.removeOper(clientIdToOper);
      channel.broadcastToAll(server, client.getClientNickName() + " removes channel operator status from " + arg +  "\r\n");
      return true;
    }

    else if (!isClientOper && flag == '+' && clientIdToOper != client.getClientId()) {
      channel.addOper(clientIdToOper);
      channel.broadcastToAll(server, client.getClientNickName() + " gives channel operator status to " + arg +  "\r\n");
      return true;
    }
    return false;
  }

  bool handleSingleMode(Server &server, Client &client, const std::string &providedChannel, char flag, char mode, const std::string &arg) {
    Channel* channel = NULL;
    std::vector<Channel>::iterator it = server.getChannelsVector().begin();

    while (it != server.getChannelsVector().end()) {
      if (it->getName() == providedChannel) {
        channel = &(*it);
        break;
      }
    }

    if (!channel)
      return false;

    switch (mode) {
      case 'i': return handleInviteMode(server, client, *channel, flag);
      case 't': return handleTopicMode(server, client, *channel, flag);
      case 'k': return handleKeyMode(server, client, *channel, flag, arg);
      case 'l': return handleLimitMode(server, client, *channel, flag, arg);
      case 'o': return handleOperatorMode(server, client, *channel, flag, arg);
      default: return false;
    }
  }

  bool handleChannelMode(Server &server, Client &client, const std::string &providedChannel, const std::string &providedModes, const std::vector<std::string> & providedModesArgs) {
    bool atLeastOneModeSucceeded = false;
    size_t argIndex = 0;

    for (size_t i = 0; i < providedModes.length(); i += 2) {
      char flag = providedModes[i];
      char mode = providedModes[i + 1];
      std::string arg = (mode == 'k' || mode == 'l' || mode == 'o') && argIndex < providedModesArgs.size() ? providedModesArgs[argIndex] : "";

      if (handleSingleMode(server, client, providedChannel, flag, mode, arg))
        atLeastOneModeSucceeded = true;

      if (mode == 'k' || mode == 'l' || mode == 'o')
        argIndex++;
    }

  return atLeastOneModeSucceeded;
  }
}

void ModeCommand::execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug) {
  std::string clientNick = client.getClientNickName();
  int clientFd = client.getClientFd();
  std::string providedChannel  = args[1];
  static_cast<void>(debug);

  if (args.size() < 3)
    return(server.sendMessage(clientFd, Messages::ERR_NEEDMOREPARAMS(clientNick, "MODE"))); // TODO handle only one arg show curr mode and infos

  if (!server.isClientRegistered(clientNick))
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHNICK(clientNick, "MODE")));

  if (!server.isChannelRegistered(providedChannel))
    return(server.sendMessage(clientFd, Messages::ERR_NOSUCHCHANNEL(clientNick, "MODE")));

  if (!server.isClientOnChannel(clientNick, providedChannel))
    return(server.sendMessage(clientFd, Messages::ERR_NOTONCHANNEL(clientNick, "MODE")));

  if (!server.isClientOperOnChannel(clientNick, providedChannel)) // TODO talvez tirar essa validação daqui para cair nos erros especificos de cada comando...
    return(server.sendMessage(clientFd, Messages::ERR_CHANOPRIVSNEEDED(clientNick, "MODE")));

  std::string providedModes = Parser::formatOperatorModes(args[2]);
  std::vector<std::string> providedModesArgs = Parser::formatOperatorModeArgs(args);

  if (!handleChannelMode(server, client ,providedChannel, providedModes, providedModesArgs))
    return(server.sendMessage(clientFd, "Error while setting mode to channel\r\n"));
}
