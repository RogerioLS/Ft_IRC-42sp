/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:33:53 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:33:54 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>

namespace Messages {
  inline std::string ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& command) {
    return ":server 461 " + nick + " " + command + " :Not enough parameters\r\n";
  }

  inline std::string ERR_ALREADYREGISTRED(const std::string& nick) {
    return ":server 462 " + nick + " :You may not reregister\r\n";
  }

  inline std::string ERR_PASSWDMISMATCH(const std::string& nick) {
    return ":server 464 " + nick + " :Password incorrect\r\n";
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

  inline std::string RPL_WELCOME(const std::string& nick) {
    return ":server 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n";
  }
}

#endif
