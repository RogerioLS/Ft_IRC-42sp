#include "TestHelper.hpp"

// Teste para o comando TOPIC
TEST_F(CommandTest, TopicCommand) {
    // Configuração do canal
    server->createChannel("#test", *client1);
    Channel* channel = server->getChannelByName("#test");
    channel->setClientsById(client2->getClientId());

    // user1 (operador) define o tópico
    commandHandler->executeCommand(*client1, "TOPIC #test :New topic");

    // Verifica se o tópico foi definido e a mensagem enviada a todos no canal
    ASSERT_EQ(channel->getTopic(), "New topic");
    ASSERT_EQ(server->sent_messages.size(), 2); // Mensagem para user1 e user2
    std::string expected_msg_topic = ":user1!userone@127.0.0.1 TOPIC #test :New topic\r\n";
    ASSERT_EQ(cleanMessage(server->sent_messages[0].substr(server->sent_messages[0].find("msg:") + 4)), cleanMessage(expected_msg_topic));
    ASSERT_EQ(cleanMessage(server->sent_messages[1].substr(server->sent_messages[1].find("msg:") + 4)), cleanMessage(expected_msg_topic));
}

// Teste para o comando TOPIC em canal com tópico restrito
TEST_F(CommandTest, TopicCommandRestricted) {
    server->createChannel("#moderated", *client1);
    Channel* channel = server->getChannelByName("#moderated");
    channel->setClientsById(client2->getClientId());
    channel->setRestrictTopic(true); // Apenas operadores podem mudar o tópico

    // client2 (não operador) tenta mudar o tópico
    commandHandler->executeCommand(*client2, "TOPIC #moderated :New topic by user2");
    ASSERT_EQ(channel->getTopic(), "");
    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string error_msg = "to:2 msg::server 482 user2 #moderated :You're not channel operator\r\n";
    ASSERT_EQ(server->sent_messages[0], error_msg);

    server->sent_messages.clear();

    // client1 (operador) muda o tópico com sucesso
    commandHandler->executeCommand(*client1, "TOPIC #moderated :Official topic");
    ASSERT_EQ(channel->getTopic(), "Official topic");
    ASSERT_EQ(server->sent_messages.size(), 2); // Mensagem para user1 e user2
}

// Teste para o comando TOPIC para ver o tópico
TEST_F(CommandTest, TopicCommandViewTopic) {
    server->createChannel("#news", *client1);
    Channel* channel = server->getChannelByName("#news");
    channel->setTopic("Today's news");

    // client1 pede para ver o tópico
    commandHandler->executeCommand(*client1, "TOPIC #news");

    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string topic_msg = "to:1 msg::irc.mock 332 user1 #news :Today's news\r\n";
    ASSERT_EQ(server->sent_messages[0], topic_msg);
}

// Teste para o comando TOPIC em um canal que não existe
TEST_F(CommandTest, TopicCommandNoChannel) {
    commandHandler->executeCommand(*client1, "TOPIC #no-such-channel :some topic");

    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string error_msg = "to:1 msg::server 403 user1 TOPIC :No such channel\r\n";
    ASSERT_EQ(server->sent_messages[0], error_msg);
}

// Teste para o comando TOPIC sem estar no canal
TEST_F(CommandTest, TopicCommandNotOnChannel) {
    server->createChannel("#somechannel", *client2); // client2 cria, client1 não está nele

    commandHandler->executeCommand(*client1, "TOPIC #somechannel :a new topic");

    ASSERT_EQ(server->sent_messages.size(), 1);
    std::string error_msg = "to:1 msg::server 482 user1 #somechannel :You're not channel operator\r\n";
    ASSERT_EQ(server->sent_messages[0], error_msg);
}