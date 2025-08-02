/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:53:53 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:55:14 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_COMMAND_HPP
#define MODE_COMMAND_HPP

#include "../server/Server.hpp"
#include "../utils/IRC.hpp"

class ModeCommand {

  public:
    static void execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
