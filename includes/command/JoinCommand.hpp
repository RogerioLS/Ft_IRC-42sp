#ifndef JOINCOMMAND_HPP
#define JOINCOMMAND_HPP

#include "../../includes/server/IServer.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Debug.hpp"
#include <vector>
#include <string>

class JoinCommand {
public:
    static void execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
