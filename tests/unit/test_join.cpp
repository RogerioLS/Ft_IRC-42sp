#include "TestHelper.hpp"

// Teste para o comando JOIN
TEST_F(CommandTest, JoinCommand) {
    commandHandler->executeCommand(*client1, "JOIN #newchannel");

    // Verifica se o canal foi criado e o cliente adicionado
    Channel* channel = server->getChannelByName("#newchannel");
    ASSERT_TRUE(channel != NULL);
    ASSERT_TRUE(channel->isClientInChannel(client1->getClientId()));

    // Verifica as mensagens de boas-vindas
    ASSERT_EQ(server->sent_messages.size(), 3);
    std::string join_msg = "to:1 msg::user1!userone@127.0.0.1 JOIN #newchannel\r\n";
    std::string expected_namreply_msg = "to:1 msg::server 353 user1 = #newchannel :@user1 \r\n";
    std::string expected_endofnames_msg = "to:1 msg::server 366 user1 #newchannel :End of /NAMES list.\r\n";
    
    ASSERT_EQ(server->sent_messages[0], join_msg);
    ASSERT_EQ(server->sent_messages[1], expected_namreply_msg);
}

// Teste para o comando JOIN com senha
TEST_F(CommandTest, JoinCommandWithPassword) {
    // Configura o canal com senha
    server->createChannel("#secure", *client2);
    Channel* channel = server->getChannelByName("#secure");
    
    channel->setKey("secret");

    // Tentativa de entrar sem senha (deve falhar)
    commandHandler->executeCommand(*client1, "JOIN #secure");
    ASSERT_FALSE(channel->isClientInChannel(client1->getClientId()));
    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string error_msg = "to:1 msg::server 475 user1 #secure :Cannot join channel (+k)\r\n";
    ASSERT_EQ(server->sent_messages[0], error_msg);

    server->sent_messages.clear(); // Limpa as mensagens para o próximo teste

    // Tentativa de entrar com a senha correta
    commandHandler->executeCommand(*client1, "JOIN #secure secret");
    ASSERT_TRUE(channel->isClientInChannel(client1->getClientId()));
    ASSERT_EQ(server->sent_messages.size(), 3); // JOIN, TOPIC, NAMES
}

// Teste para o comando JOIN em canal apenas para convidados
TEST_F(CommandTest, JoinCommandInviteOnly) {
    // Configura o canal como apenas para convidados
    server->createChannel("#private", *client2);
    Channel* channel = server->getChannelByName("#private");
    channel->setInviteOnly(true);

    // Tentativa de entrar sem convite (deve falhar)
    commandHandler->executeCommand(*client1, "JOIN #private");
    ASSERT_FALSE(channel->isClientInChannel(client1->getClientId()));
    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string error_msg = "to:1 msg::server 473 user1 #private :Cannot join channel (+i)\r\n";
    ASSERT_EQ(server->sent_messages[0], error_msg);

    server->sent_messages.clear();

    // Convidar e tentar entrar novamente
    channel->setClientsInvitedById(client1->getClientId());
    commandHandler->executeCommand(*client1, "JOIN #private");
    ASSERT_TRUE(channel->isClientInChannel(client1->getClientId()));
    ASSERT_EQ(server->sent_messages.size(), 3);
}

// Teste para o comando JOIN em canal cheio
TEST_F(CommandTest, JoinCommandChannelFull) {
    // Configura o canal com limite de 1 usuário
    server->createChannel("#full", *client2);
    Channel* channel = server->getChannelByName("#full");
    channel->setUserLimit(1);

    // Tentativa de entrar no canal cheio (deve falhar)
    commandHandler->executeCommand(*client1, "JOIN #full");
    ASSERT_FALSE(channel->isClientInChannel(client1->getClientId()));
    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string error_msg = "to:1 msg::server 471 user1 #full :Cannot join channel (+l)\r\n";
    ASSERT_EQ(server->sent_messages[0], error_msg);
}
