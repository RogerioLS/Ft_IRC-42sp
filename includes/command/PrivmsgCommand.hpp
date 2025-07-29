/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:35:05 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:35:26 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_COMMAND_HPP
#define PRIVMSG_COMMAND_HPP

#include "../server/Server.hpp"
#include "../utils/IRC.hpp"

class PrivmsgCommand {

  public:
    static void execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
