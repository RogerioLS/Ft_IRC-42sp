#include <gtest/gtest.h>
#include "../../includes/command/CommandHandler.hpp"
#include "../../includes/server/IServer.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Debug.hpp"
#include "../../includes/utils/Messages.hpp"
#include <map>
#include <string>
#include <vector>

// Helper para limpar mensagens
std::string cleanMessage(std::string msg) {
    if (!msg.empty() && msg.back() == '\n') {
        msg.pop_back();
    }
    if (!msg.empty() && msg.back() == '\r') {
        msg.pop_back();
    }
    return msg;
}

// Mock do Servidor para testes de unidade
class ServerMock : public IServer {
public:
	Debug& _debug;
	std::map<std::string, Channel> _channels;
	std::map<int, Client> _clients;
	std::string _serverName;
	std::string _password;
	std::vector<std::string> sent_messages; // Para rastrear mensagens enviadas

	ServerMock(Debug& debug) : _debug(debug), _serverName("irc.mock") {
		// Adiciona clientes mock
		_clients.emplace(1, Client(1, 1));
		_clients.at(1).setClientNickName("user1"); // Nick temporário até o registro
		_clients.at(1).setClientUserName("username1");


		_clients.emplace(2, Client(2, 2));
		_clients.at(2).setClientNickName("user2");
		_clients.at(2).setClientUserName("username2");
		_clients.at(2).setHasValidPass(true);
		_clients.at(2).setHasValidNick(true);
		_clients.at(2).setHasValidUser(true);
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

	const std::string& getPassword() const override {
		return _password;
	}

	// Mock para envio de mensagens
	void sendMessage(int fd, const std::string& message) override {
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
		debug->enableDebugMode(true); // HABILITA DEBUG
		serverMock = new ServerMock(*debug);
		serverMock->_password = "password"; // Define a senha do servidor mock
		commandHandler = new CommandHandler(*serverMock, *debug);

		client1 = serverMock->getClientById(1);
		client2 = serverMock->getClientById(2);

		// Garante que o client1 comece não registrado
		client1->setHasValidPass(false);
		client1->setHasValidNick(false);
		client1->setHasValidUser(false);
	}

	void TearDown() override {
		delete commandHandler;
		delete serverMock;
		delete debug;
	}
};

// Testes de Autenticação
TEST_F(CommandTest, PassCommand_CorrectPassword) {
	commandHandler->executeCommand(*client1, "PASS password\r\n");
	EXPECT_TRUE(client1->hasValidPass());
	EXPECT_TRUE(serverMock->getSentMessages().empty()); // Nenhuma mensagem em caso de sucesso
}

TEST_F(CommandTest, PassCommand_IncorrectPassword) {
	commandHandler->executeCommand(*client1, "PASS wrongpassword\r\n");
	EXPECT_FALSE(client1->hasValidPass());
	ASSERT_EQ(serverMock->getSentMessages().size(), 1);
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg:" + Messages::ERR_PASSWDMISMATCH(client1->getClientNickName())));
}

TEST_F(CommandTest, NickCommand_ValidNick) {
	client1->setHasValidPass(true); // Simula que o PASS foi bem-sucedido
	commandHandler->executeCommand(*client1, "NICK new_nick\r\n");
	EXPECT_TRUE(client1->hasValidNick());
	EXPECT_EQ(client1->getClientNickName(), "new_nick");
	EXPECT_TRUE(serverMock->getSentMessages().empty());
}

TEST_F(CommandTest, NickCommand_NicknameInUse) {
	client1->setHasValidPass(true);
	commandHandler->executeCommand(*client1, "NICK user2\r\n"); // user2 já existe
	EXPECT_FALSE(client1->hasValidNick());
	ASSERT_EQ(serverMock->getSentMessages().size(), 1);
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg:" + Messages::ERR_NICKNAMEINUSE(client1->getClientNickName(), "user2")));
}

TEST_F(CommandTest, NickCommand_NoPassword) {
	commandHandler->executeCommand(*client1, "NICK new_nick\r\n");
	EXPECT_FALSE(client1->hasValidNick());
	ASSERT_EQ(serverMock->getSentMessages().size(), 1);
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg::server NOTICE * :You must provide a password first."));
}

TEST_F(CommandTest, UserCommand_ValidUser) {
	client1->setHasValidPass(true);
	client1->setHasValidNick(true);
	client1->setClientNickName("new_nick");
	commandHandler->executeCommand(*client1, "USER user host server :Real Name\r\n");
	EXPECT_TRUE(client1->isFullyRegistered());
	EXPECT_EQ(client1->getClientUserName(), "user");
	EXPECT_EQ(client1->getClientRealName(), "Real Name");
	ASSERT_EQ(serverMock->getSentMessages().size(), 1);
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg:" + Messages::RPL_WELCOME(client1->getClientNickName())));
}

TEST_F(CommandTest, UserCommand_NotEnoughParams) {
	client1->setHasValidPass(true);
	client1->setHasValidNick(true);
	commandHandler->executeCommand(*client1, "USER user host\r\n");
	EXPECT_FALSE(client1->isFullyRegistered());
	ASSERT_EQ(serverMock->getSentMessages().size(), 1);
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg:" + Messages::ERR_NEEDMOREPARAMS(client1->getClientNickName(), "USER")));
}

TEST_F(CommandTest, FullRegistration_Success) {
	// 1. PASS
	commandHandler->executeCommand(*client1, "PASS password\r\n");
	ASSERT_TRUE(client1->hasValidPass());

	// 2. NICK
	commandHandler->executeCommand(*client1, "NICK final_nick\r\n");
	ASSERT_TRUE(client1->hasValidNick());
	ASSERT_EQ(client1->getClientNickName(), "final_nick");

	// 3. USER
	commandHandler->executeCommand(*client1, "USER u h s :Realest Name\r\n");
	ASSERT_TRUE(client1->isFullyRegistered());

	// Verifica a mensagem de boas-vindas
	ASSERT_EQ(serverMock->getSentMessages().size(), 1);
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg:" + Messages::RPL_WELCOME("final_nick")));
}


// Testes JOIN
TEST_F(CommandTest, JoinCommand_NewChannel) {
	commandHandler->executeCommand(*client2, "JOIN #test\r\n");
	Channel* channel = serverMock->getChannelByName("#test");
	ASSERT_TRUE(channel != nullptr);
	EXPECT_TRUE(channel->isClientInChannel(client2->getClientId()));
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
	commandHandler->executeCommand(*client2, "PRIVMSG user1 :Hello\r\n");
	ASSERT_FALSE(serverMock->getSentMessages().empty());
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg::user2!username2@ PRIVMSG user1 :Hello"));
}

TEST_F(CommandTest, PrivmsgCommand_ToChannel) {
	commandHandler->executeCommand(*client2, "JOIN #test\r\n");
	serverMock->sent_messages.clear(); // Limpa mensagens anteriores do JOIN
	commandHandler->executeCommand(*client2, "PRIVMSG #test :Hello all\r\n");
	ASSERT_EQ(serverMock->getSentMessages().size(), 0); // Nenhum outro cliente no canal
}

// Testes TOPIC
TEST_F(CommandTest, TopicCommand_SetAndGetTopic) {
	commandHandler->executeCommand(*client2, "JOIN #test\r\n");
	commandHandler->executeCommand(*client2, "TOPIC #test :New Topic!\r\n");
	Channel* channel = serverMock->getChannelByName("#test");
	ASSERT_TRUE(channel != nullptr);
	EXPECT_EQ(channel->getTopic(), "New Topic!");
}

TEST_F(CommandTest, TopicCommand_GetEmptyTopic) {
	commandHandler->executeCommand(*client2, "JOIN #test\r\n");
	serverMock->sent_messages.clear(); // Limpa mensagens anteriores do JOIN
	commandHandler->executeCommand(*client2, "TOPIC #test\r\n");
	ASSERT_FALSE(serverMock->getSentMessages().empty());
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:2 msg::irc.mock 331 user2 #test :No topic is set"));
}

// Teste HELP
TEST_F(CommandTest, HelpCommand) {
	commandHandler->executeCommand(*client1, "HELP\r\n");
	ASSERT_EQ(serverMock->getSentMessages().size(), 5); // Espera 5 partes da mensagem de ajuda
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[0]), cleanMessage("to:1 msg::irc.mock NOTICE user1 :Available commands:"));
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[1]), cleanMessage("to:1 msg::irc.mock NOTICE user1 :  JOIN <#channel> - Join or create a channel."));
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[2]), cleanMessage("to:1 msg::irc.mock NOTICE user1 :  PRIVMSG <target> :<message> - Send a private message to a user or channel."));
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[3]), cleanMessage("to:1 msg::irc.mock NOTICE user1 :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel."));
	EXPECT_EQ(cleanMessage(serverMock->getSentMessages()[4]), cleanMessage("to:1 msg::irc.mock NOTICE user1 :  HELP - Shows this help message."));
}