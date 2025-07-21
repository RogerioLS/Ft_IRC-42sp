#include <gtest/gtest.h>
#include "../../includes/command/CommandHandler.hpp"
#include "../../includes/server/IServer.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Debug.hpp"
#include <map>
#include <string>
#include <vector>

// Mock do Servidor para testes de unidade
class ServerMock : public IServer {
public:
	Debug& _debug;
	std::map<std::string, Channel> _channels;
	std::map<int, Client> _clients;
	std::string _serverName;
	std::vector<std::string> sent_messages; // Para rastrear mensagens enviadas

	ServerMock(Debug& debug) : _debug(debug), _serverName("irc.mock") {
		// Adiciona clientes mock
		_clients.emplace(1, Client(1, 1));
		_clients.at(1).setClientNickName("user1");
		_clients.at(1).setClientUserName("username1");

		_clients.emplace(2, Client(2, 2));
		_clients.at(2).setClientNickName("user2");
		_clients.at(2).setClientUserName("username2");
	}

	~ServerMock() override {}

	// Implementação da interface IServer
	Channel* getChannelByName(const std::string& name) override {
		auto it = _channels.find(name);
		return (it != _channels.end()) ? &it->second : nullptr;
	}

	Client* getClientByNickname(const std::string& nickname) override {
		for (auto& pair : _clients) {
			if (pair.second.getClientNickName() == nickname) {
				return &pair.second;
			}
		}
		return nullptr;
	}

	Client* getClientById(int id) override {
		auto it = _clients.find(id);
		return (it != _clients.end()) ? &it->second : nullptr;
	}

	void createChannel(const std::string& name, Client& client) override {
		if (_channels.find(name) == _channels.end()) {
			_channels.emplace(name, Channel(name, _channels.size() + 1, client.getClientId(), _debug));
		}
	}

	const std::string& getServerName() const override {
		return _serverName;
	}

	// Mock para envio de mensagens
	void sendMessage(int fd, const std::string& message) {
		sent_messages.push_back("to:" + std::to_string(fd) + " msg:" + message);
	}

	const std::vector<std::string>& getSentMessages() const {
		return sent_messages;
	}
};

// Test Fixture
class CommandTest : public ::testing::Test {
protected:
	ServerMock* serverMock;
	Debug* debug;
	CommandHandler* commandHandler;
	Client* client1;
	Client* client2;

	void SetUp() override {
		debug = new Debug();
		debug->enableDebugMode(false);
		serverMock = new ServerMock(*debug);
		commandHandler = new CommandHandler(*serverMock, *debug);

		client1 = serverMock->getClientById(1);
		client2 = serverMock->getClientById(2);
	}

	void TearDown() override {
		delete commandHandler;
		delete serverMock;
		delete debug;
	}
};

// Testes JOIN
TEST_F(CommandTest, JoinCommand_NewChannel) {
	commandHandler->executeCommand(*client1, "JOIN #test\r\n");
	Channel* channel = serverMock->getChannelByName("#test");
	ASSERT_TRUE(channel != nullptr);
	EXPECT_TRUE(channel->isClientInChannel(client1->getClientId()));
}

TEST_F(CommandTest, JoinCommand_ExistingChannel) {
	commandHandler->executeCommand(*client1, "JOIN #test\r\n");
	commandHandler->executeCommand(*client2, "JOIN #test\r\n");
	Channel* channel = serverMock->getChannelByName("#test");
	ASSERT_TRUE(channel != nullptr);
	EXPECT_TRUE(channel->isClientInChannel(client2->getClientId()));
	EXPECT_EQ(channel->getClientsById().size(), 2);
}

// Testes PRIVMSG
TEST_F(CommandTest, PrivmsgCommand_ToUser) {
	commandHandler->executeCommand(*client1, "PRIVMSG user2 :Hello\r\n");
	ASSERT_FALSE(serverMock->getSentMessages().empty());
	EXPECT_EQ(serverMock->getSentMessages()[0], "to:2 msg::user1!username1@ PRIVMSG user2 :Hello\r\n");
}

TEST_F(CommandTest, PrivmsgCommand_ToChannel) {
	commandHandler->executeCommand(*client1, "JOIN #test\r\n");
	commandHandler->executeCommand(*client2, "JOIN #test\r\n");
	serverMock->sent_messages.clear(); // Limpa mensagens anteriores do JOIN
	commandHandler->executeCommand(*client1, "PRIVMSG #test :Hello all\r\n");
	ASSERT_EQ(serverMock->getSentMessages().size(), 1); // Espera 1 mensagem (para client2, remetente não recebe)
	EXPECT_EQ(serverMock->getSentMessages()[0], "to:2 msg::user1!username1@ PRIVMSG #test :Hello all\r\n");
}

// Testes TOPIC
TEST_F(CommandTest, TopicCommand_SetAndGetTopic) {
	commandHandler->executeCommand(*client1, "JOIN #test\r\n");
	commandHandler->executeCommand(*client1, "TOPIC #test :New Topic!\r\n");
	Channel* channel = serverMock->getChannelByName("#test");
	ASSERT_TRUE(channel != nullptr);
	EXPECT_EQ(channel->getTopic(), "New Topic!");
}

TEST_F(CommandTest, TopicCommand_GetEmptyTopic) {
	commandHandler->executeCommand(*client1, "JOIN #test\r\n");
	serverMock->sent_messages.clear(); // Limpa mensagens anteriores do JOIN
	commandHandler->executeCommand(*client1, "TOPIC #test\r\n");
	ASSERT_FALSE(serverMock->getSentMessages().empty());
	EXPECT_EQ(serverMock->getSentMessages()[0], "to:1 msg::irc.mock 331 user1 #test :No topic is set\r\n");
}

// Teste HELP
TEST_F(CommandTest, HelpCommand) {
	commandHandler->executeCommand(*client1, "HELP\r\n");
	ASSERT_EQ(serverMock->getSentMessages().size(), 5); // Espera 5 partes da mensagem de ajuda
	EXPECT_EQ(serverMock->getSentMessages()[0], "to:1 msg::irc.mock NOTICE user1 :Available commands:\r\n");
	EXPECT_EQ(serverMock->getSentMessages()[1], "to:1 msg::irc.mock NOTICE user1 :  JOIN <#channel> - Join or create a channel.\r\n");
	EXPECT_EQ(serverMock->getSentMessages()[2], "to:1 msg::irc.mock NOTICE user1 :  PRIVMSG <target> :<message> - Send a private message to a user or channel.\r\n");
	EXPECT_EQ(serverMock->getSentMessages()[3], "to:1 msg::irc.mock NOTICE user1 :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel.\r\n");
	EXPECT_EQ(serverMock->getSentMessages()[4], "to:1 msg::irc.mock NOTICE user1 :  HELP - Shows this help message.\r\n");
}