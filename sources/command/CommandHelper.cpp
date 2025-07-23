/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHelper.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:39:38 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/07/23 12:25:15 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/command/CommandHandler.hpp"
#include "../includes/server/Client.hpp"
#include "../includes/server/Server.hpp"

void CommandHandler::sendResponse(Client &client, const std::string &response) {
    send(client.getClientFd(), response.c_str(), response.length(), 0);
}

bool CommandHandler::isValidNickname(const std::string &nickname) {
    if (nickname.empty() || nickname.length() > 9) {
        return false;
    }

    if (!std::isalpha(nickname[0])) {
        return false;
    }

    for (size_t i = 1; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!std::isalnum(c) && c != '_' && c != '-' && c != '[' && c != ']' && c != '\\' && c != '`' && c != '^' && c != '{' && c != '}') {
            return false;
        }
    }
    return true;
}

bool CommandHandler::isNicknameInUse(const std::string &nickname, Server &server) {
    std::vector<Client> &clients = server.getClientsVector();
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getClientNickName() == nickname && it->hasValidNick()) {
            return true;
        }
    }
    return false;
}

bool CommandHandler::executeOperKick(Client &client, const std::vector<std::string> &args, Server &server) {
  if (args.size() < 3)
    return (sendResponse(client, "461 KICK :Not enough parameters\r\n"), false);

  std::string reasonToKick;
  std::string clientNick = client.getClientNickName();
  std::vector<std::string> channelsFromKick = Parser::splitCommandArg(args[1], ",");
  std::vector<std::string> clientsToKick = Parser::splitCommandArg(args[2], ",");
  bool atLeastOneKickSucceeded = false;

  if (args.size() > 3)
    reasonToKick = args[3];
  else
    reasonToKick = "";

  for (size_t i = 0; i < channelsFromKick.size(); ++i) {
    std::string providedChannel = channelsFromKick[i];
    if (server.isChannelRegistered(providedChannel) && (sendResponse(client, "403 ERR_NOSUCHCHANNEL " + providedChannel + " :No such channel\r\n"), false))
      continue;
    if (server.isClientOnChannel(clientNick, providedChannel) && (sendResponse(client, "442 ERR_NOTONCHANNEL " + providedChannel + " :You're not on that channel\r\n"), false))
      continue;
    if (server.isClientOperOnChannel(clientNick, providedChannel) && (sendResponse(client, "482 ERR_CHANOPRIVSNEEDED " + providedChannel + " :You're not channel operator\r\n"), false))
      continue;
    for (size_t j = 0; j < clientsToKick.size(); ++j) {
      std::string providedClientToKick = clientsToKick[j];
      if (server.isClientRegistered(providedClientToKick) && (sendResponse(client, "401 ERR_NOSUCHNICK " + providedClientToKick + " :No such nick/channel\r\n"), false))
        continue;
      if (server.isClientOnChannel(providedClientToKick, providedChannel) && (sendResponse(client, "441 ERR_USERNOTINCHANNEL " + providedClientToKick + " " + providedChannel + " :They aren't on that channel\r\n"), false))
        continue;
      if (kickClientFromChannel(server, providedChannel, providedClientToKick, clientNick, reasonToKick) && (sendResponse(client, "Unknown error kicking " + providedClientToKick + "\r\n"), false))
        continue;

      sendResponse(client, "KICK successful for " + providedClientToKick + " on " + providedChannel + "\r\n");
      atLeastOneKickSucceeded = true;
    }
  }
  return atLeastOneKickSucceeded;
}

bool CommandHandler::kickClientFromChannel(Server &server,  const std::string &providedChannel, const std::string &providedClientToKick, const std::string & providedOper, const std::string & reasonToKick) {
  std::vector<Channel>::iterator it = server.getChannelsVector().begin();

  while (it != server.getChannelsVector().end()) {
    if (it->getName() == providedChannel) {
      int clientToKickId = server.getClientIdFromNickname(providedClientToKick);
      it->removeClient(clientToKickId);
      std::string kickMsg = reasonToKick;
      if (!kickMsg.empty())
        kickMsg += " : " + reasonToKick;
      else
        kickMsg += providedOper + "(" + providedOper + ")";
      sendResponse(*const_cast<Client*>(server.getClientInstFromId(clientToKickId)), "You have been kicked from " + providedChannel + " by " + providedOper + "\r\n");
      // std::string broadcastMsg = ":" + targetClient->getNick() + " KICK " + providedChannel + " :" + targetClient->getNick() + "\r\n";
      // it->broadcastMessage(broadcastMsg, *targetClient); TODO
      return true;
    }
    ++it;
  }
  return false;
}

bool CommandHandler::executeOperInvite(Client &client, const std::vector<std::string> &args, Server &server) {
  if (args.size() < 3)
    return (sendResponse(client, "461: OPER :Not enough parameters\r\n"), false);

  std::string providedClientToInvite = args[1];
  std::string providedChannel = args[2];
  std::string clientNick = client.getClientNickName();

  if (!server.isClientRegistered(providedClientToInvite))
    return(sendResponse(client, "401 : OPER :<nickname> :No such nick/channel\r\n"), false);
  if(!server.isChannelRegistered(providedChannel))
    return(sendResponse(client, "403 ERR_NOSUCHCHANNEL " + providedChannel + " :No such channel\r\n"), false);
  if (!server.isClientOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "442 : OPER : <channel> :You're not on that channel\r\n"), false);
  if (!server.isClientOperOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "482 : OPER : <channel> : :You're not channel operator\r\n"), false);
  if (!server.isClientOnChannel(providedClientToInvite, providedChannel))
    return(sendResponse(client, "443: OPER : <user> <channel> :is already on channel\r\n"), false);
  if (!inviteClientToChannel(server, providedChannel, providedClientToInvite, clientNick))
    return(sendResponse(client, "Unknown Error while Inviting Client\r\n"), false);

  sendResponse(client, "341 : OPER : <channel> <nick>\r\n");
  return true;
}

bool CommandHandler::inviteClientToChannel(Server &server, const std::string & providedChannel, const std::string & providedClientToInvite, const std::string & providedOper ) {
  std::vector<Channel>::iterator it = server.getChannelsVector().begin();
  while(it != server.getChannelsVector().end()) {
    if (it->getName() == providedChannel) {
      int providedClientToInviteId = server.getClientIdFromNickname(providedClientToInvite);
      it->setClientsInvitedById(providedClientToInviteId);
      sendResponse(*const_cast<Client*>(server.getClientInstFromId(providedClientToInviteId)), "You have been invited to " +  providedChannel +  "by " + providedOper);
      return true;
      }
    ++it;
  }
  return false;
}

bool CommandHandler::executeChannelMode(Client &client, const std::vector<std::string> &args, Server &server) {
  if (args.size() < 3)
    return (sendResponse(client, "461 USER :Not enough parameters\r\n"), false);

  std::string providedChannel  = args[1];
  std::string clientNick = client.getClientNickName();

  if (!server.isClientRegistered(clientNick))
    return(sendResponse(client, "401 : OPER :<nickname> :No such nick/channel\r\n"), false);
  if (!server.isChannelRegistered(providedChannel))
    return(sendResponse(client, "403 ERR_NOSUCHCHANNEL " + providedChannel + " :No such channel\r\n"), false);
  if (!server.isClientOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "442 : OPER : <channel> :You're not on that channel\r\n"), false);
  if (!server.isClientOperOnChannel(clientNick, providedChannel))
    return(sendResponse(client, "482 : OPER : <channel> : :You're not channel operator\r\n"), false);

  std::string providedModes = Parser::formatOperatorModes(args[2]);
  std::vector<std::string> providedModesArgs = Parser::formatOperatorModeArgs(args);

  if (!handleChannelMode(server, client ,providedChannel, providedModes, providedModesArgs))
    return(sendResponse(client, "Error while setting mode to channel\r\n"), false);

  return true;
}

bool CommandHandler::handleChannelMode(Server &server, Client &client, const std::string &providedChannel, const std::string &providedModes, const std::vector<std::string> & providedModesArgs) {
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

bool CommandHandler::handleSingleMode(Server &server, Client &client, const std::string &providedChannel, char flag, char mode, const std::string &arg) {
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

bool CommandHandler::handleInviteMode(Server &server, Client &client, Channel &channel, char flag) {
  if (channel.getInviteOnly() == true) {
    if (flag == '-') {
      channel.setInviteOnly(false);
      return true;
      //LOGS
    }
  } else if (channel.getInviteOnly() == false) {
    if (flag == '+') {
      channel.setInviteOnly(true);
      return true;
      //LOGS
    }
  }
  return false;
}

bool CommandHandler::handleTopicMode(Server &server, Client &client, Channel &channel, char flag) {

  if (channel.getRestrictTopic() == true) {
    if (flag == '-') {
      channel.setRestrictTopic(false);
      return true;
      //LOGS
    }
  } else if (channel.getRestrictTopic() == false) {
    if (flag == '+') {
      channel.setRestrictTopic(true);
      return true;
      //LOGS
    }
  }
  return false;
}

bool CommandHandler::handleKeyMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {

  if (arg.length() > 0) {
    if (flag == '-' && channel.getKey() == arg) {
      channel.setKey("");
      return true;
      //LOGS
    }
    else if (flag == '-' && channel.getKey() != arg) {
      return false;
      //LOGS
    }
    else if (flag == '+' && channel.getKey().length() == 0) {
      channel.setKey(arg);
      return true;
      //LOGS
    }
    else if (flag == '+' && channel.getKey().length() > 0) {
      return false;
      //LOGS
    }
  }
  else {
    //LOG #bla k * :You must specify a parameter for the key mode. Syntax: <key>.
  }
}

bool CommandHandler::handleLimitMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {
  if (flag && !arg.empty()) {
    return true;
  } else if (!flag) {
    return true;
  }
  return false;
}

bool CommandHandler::handleOperatorMode(Server &server, Client &client, Channel &channel, char flag, const std::string &arg) {

  if (!server.isClientRegistered(arg))
    return(sendResponse(client, "401 : OPER :<nickname> :No such nick/channel\r\n"), false);

  int clientIdToOper = server.getClientIdFromNickname(arg);
  std::set<int> channelOpers = channel.getOperatorsById();
  bool isClientOper = (channelOpers.find(clientIdToOper) != channelOpers.end());

  if (flag == '-' && isClientOper) {
    channel.removeOper(clientIdToOper);
    return true;
    //LOGS
  } else if (flag == '+' && !isClientOper) {
    channel.addOper(clientIdToOper);
    return true;
    //LOGS
  }
  return false;
}
