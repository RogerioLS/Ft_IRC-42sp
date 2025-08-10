#include "TestHelper.hpp"

// Teste para o comando HELP
TEST_F(CommandTest, HelpCommand) {
    std::vector<std::string> args;
    commandHandler->executeCommand(*client1, "HELP");
    ASSERT_EQ(server->sent_messages.size(), 5); // Espera 5 mensagens
    std::string serverName = server->getServerName();
    std::string nickName = client1->getClientNickName();

    std::string expected_msg_0 = "to:1 msg::" + serverName + " NOTICE " + nickName + " :Available commands:\r\n";
    std::string expected_msg_1 = "to:1 msg::" + serverName + " NOTICE " + nickName + " :  JOIN <#channel> - Join or create a channel.\r\n";
    std::string expected_msg_2 = "to:1 msg::" + serverName + " NOTICE " + nickName + " :  PRIVMSG <target> :<message> - Send a private message to a user or channel.\r\n";
    std::string expected_msg_3 = "to:1 msg::" + serverName + " NOTICE " + nickName + " :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel.\r\n";
    std::string expected_msg_4 = "to:1 msg::" + serverName + " NOTICE " + nickName + " :  HELP - Shows this help message.\r\n";

    EXPECT_EQ(server->sent_messages[0], expected_msg_0);
    EXPECT_EQ(server->sent_messages[1], expected_msg_1);
    EXPECT_EQ(server->sent_messages[2], expected_msg_2);
    EXPECT_EQ(server->sent_messages[3], expected_msg_3);
    EXPECT_EQ(server->sent_messages[4], expected_msg_4);
}

