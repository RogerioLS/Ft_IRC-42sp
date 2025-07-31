/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:33:53 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/30 18:44:52 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>

namespace Messages {
  inline std::string RPL_WELCOME(const std::string& nick) {
    return ":server 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n";
  }

  inline std::string RPL_INVITING(const std::string& channel, const std::string& target) {
    return ":server 341 " + target + " " + channel + "\r\n";
  }

  inline std::string ERR_NOSUCHNICK(const std::string& nick, const std::string& target) {
    return ":server 401 " + nick + " " + target + " :No such nick/channel\r\n";
  }

  inline std::string ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel) {
    return ":server 403 " + nick + " " + channel + " :No such channel\r\n";
  }

  inline std::string ERR_NONICKNAMEGIVEN(const std::string& nick) {
    return ":server 431 " + nick + " :No nickname given\r\n";
  }

  inline std::string ERR_ERRONEUSNICKNAME(const std::string& nick, const std::string& newNick) {
    return ":server 432 " + nick + " " + newNick + " :Erroneous nickname\r\n";
  }

  inline std::string ERR_NICKNAMEINUSE(const std::string& nick, const std::string& newNick) {
    return ":server 433 " + nick + " " + newNick + " :Nickname is already in use\r\n";
  }

  inline std::string ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& channel) {
    return ":server 441 " + nick + " " + channel + " :They aren't on that channel\r\n";
  }

  inline std::string ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel) {
    return ":server 442 " + nick + " " + channel + " :You're not on that channel\r\n";
  }

  inline std::string ERR_USERONCHANNEL(const std::string& nick, const std::string& user, const std::string& channel) {
    return ":server 443 " + nick + " " + user + " " + channel + " :is already on channel\r\n";
  }

  inline std::string ERR_NOTREGISTERED(const std::string& nick) {
    return ":server 451 " + nick + + " :You have not registered\r\n";
  }

  inline std::string ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& command) {
    return ":server 461 " + nick + " " + command + " :Not enough parameters\r\n";
  }

  inline std::string ERR_ALREADYREGISTRED(const std::string& nick) {
    return ":server 462 " + nick + " :You may not reregister\r\n";
  }

  inline std::string ERR_PASSWDMISMATCH(const std::string& nick) {
    return ":server 464 " + nick + " :Password incorrect\r\n";
  }

  inline std::string ERR_BADCHANMASK(const std::string& nick, const std::string& channel, const std::string& command) {
    return ":server 476 " + nick + " " + command + " "  + channel + " :Bad Channel Mask\r\n";
  }

  inline std::string ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel) {
    return ":server 482 " + nick + " " + channel + " :You're not channel operator\r\n";
  }

}

#endif
