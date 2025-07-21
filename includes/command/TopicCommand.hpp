#ifndef TOPICCOMMAND_HPP
#define TOPICCOMMAND_HPP

#include "../../includes/server/IServer.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Debug.hpp"
#include <vector>
#include <string>

class TopicCommand {
public:
    static void execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug);
};

#endif
