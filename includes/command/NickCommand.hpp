/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:35:00 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:35:34 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_COMMAND_HPP
#define NICK_COMMAND_HPP

#include "../server/Server.hpp"
#include "../utils/IRC.hpp"

class NickCommand {

  public:
    static void execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
