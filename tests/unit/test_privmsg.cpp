#include "TestHelper.hpp"

// Teste para o comando PRIVMSG para um canal
TEST_F(CommandTest, PrivmsgCommandToChannel) {
    // Configuração do canal e clientes
    server->createChannel("#test", *client1);
    Channel* channel = server->getChannelByName("#test");
    channel->setClientsById(client2->getClientId());

    commandHandler->executeCommand(*client1, "PRIVMSG #test :Hello everyone!");

    // Verifica se a mensagem foi enviada para o client2 (não para o client1)
    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string expected_msg = "to:2 msg::user1!userone@127.0.0.1 PRIVMSG #test :Hello everyone!\r\n";
    ASSERT_EQ(server->sent_messages[0], expected_msg);
}

// Teste para o comando PRIVMSG para um usuário
TEST_F(CommandTest, PrivmsgCommandToUser) {
    commandHandler->executeCommand(*client1, "PRIVMSG user2 :Hi there!");

    // Verifica se a mensagem foi enviada para o client2
    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string expected_msg = "to:2 msg::user1!userone@127.0.0.1 PRIVMSG user2 :Hi there!\r\n";
    ASSERT_EQ(server->sent_messages[0], expected_msg);
}

