/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:53:49 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:55:45 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_COMMAND_HPP
#define KICK_COMMAND_HPP

#include "../server/Server.hpp"
#include "../utils/IRC.hpp"

class KickCommand {

  public:
    static void execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
