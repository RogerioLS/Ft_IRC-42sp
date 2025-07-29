/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:52 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:35:18 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_COMMAND_HPP
#define USER_COMMAND_HPP

#include "../server/Server.hpp"
#include "../utils/IRC.hpp"

class UserCommand {

  public:
    static void execute(Server& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
