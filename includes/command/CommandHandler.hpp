/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:54 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:58:17 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "HelpCommand.hpp"
#include "InviteCommand.hpp"
#include "JoinCommand.hpp"
#include "KickCommand.hpp"
#include "ModeCommand.hpp"
#include "NickCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "PassCommand.hpp"
#include "TopicCommand.hpp"
#include "UserCommand.hpp"

class Client;

class CommandHandler {

  private:
    typedef void (*CommandFunction)(Server&, Client&, const std::vector<std::string>&, Debug&);

    Server& _server;
    Debug&  _debug;
    std::map<std::string, CommandFunction> _commands;

    void populateCommands();

  public:
    CommandHandler(Server& server, Debug& debug);
    ~CommandHandler();

    void executeCommand(Client& client, const std::string& message);
};

#endif
