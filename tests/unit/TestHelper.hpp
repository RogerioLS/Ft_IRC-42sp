#ifndef TEST_HELPER_HPP
#define TEST_HELPER_HPP

#include <gtest/gtest.h>
#include "../../includes/command/CommandHandler.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/server/Channel.hpp"
#include "../../includes/server/Client.hpp"
#include "../../includes/utils/Debug.hpp"
#include "../../includes/utils/Messages.hpp"
#include <map>
#include <string>
#include <vector>

// Helper para limpar mensagens
inline std::string cleanMessage(std::string msg) {
    if (!msg.empty() && msg.back() == '\n') {
        msg.pop_back();
    }
    if (!msg.empty() && msg.back() == '\r') {
        msg.pop_back();
    }
    return msg;
}

// Mock do Servidor para testes de unidade
class ServerMock : public Server {
public:
    std::vector<std::string> sent_messages;
    std::vector<Client*> _clientsVector;
    std::vector<Channel> _channelsVector;

    ServerMock(char **argv, Debug& debug) : Server(argv, debug) {
        _clientsVector.reserve(10);
        _channelsVector.reserve(10);
    }

    ~ServerMock() override {
        for (std::vector<Client*>::iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
            delete *it;
        }
        _clientsVector.clear();
    }

    void sendMessage(int fd, const std::string& message) override {
        std::stringstream ss;
        ss << fd;
        sent_messages.push_back("to:" + ss.str() + " msg:" + message);
    }

    Channel* getChannelByName(const std::string& name) override {
        for (std::vector<Channel>::iterator it = _channelsVector.begin(); it != _channelsVector.end(); ++it) {
            if (it->getName() == name) {
                return &(*it);
            }
        }
        return NULL;
    }

    const Client* getClientInstFromNick(const std::string& nickname) const override {
        for (std::vector<Client*>::const_iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
            if ((*it)->getClientNickName() == nickname) {
                return *it;
            }
        }
        return NULL;
    }

    const Client* getClientInstFromId(int clientId) const override {
        for (std::vector<Client*>::const_iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
            if ((*it)->getClientId() == clientId) {
                return *it;
            }
        }
        return NULL;
    }

    void createChannel(const std::string& name, Client& client) override {
        getDebug().debugPrint("[ServerMock] Creating channel: " + name, GREEN);
        _channelsVector.push_back(Channel(name, _channelsVector.size() + 1, client.getClientId(), getDebug()));
        getDebug().debugPrint("[ServerMock] Channel " + name + " created", GREEN);
    }

    const std::string& getServerName() const override {
        static const std::string mockServerName = "irc.mock";
        return mockServerName;
    }

    const std::string& getPassword() const override {
        static const std::string mockPassword = "password";
        return mockPassword;
    }

    bool isClientFullyRegistered(const std::string & clientNickName) const override {
        for (std::vector<Client*>::const_iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
            if ((*it)->getClientNickName() == clientNickName) {
                return (*it)->isFullyRegistered();
            }
        }
        return false;
    }
};

// Fixture de teste para comandos
class CommandTest : public ::testing::Test {
protected:
    Debug* debug;
    ServerMock* server;
    Client* client1;
    Client* client2;
    CommandHandler* commandHandler;

    void SetUp() override {
        debug = new Debug();
        debug->enableDebugMode(false); // Habilite para ver logs de debug dos testes
        char* argv[] = { (char*)"./ircserv", (char*)"6667", (char*)"password", NULL };
        server = new ServerMock(argv, *debug);
        commandHandler = new CommandHandler(*server, *debug);

        // Adiciona clientes ao mock do servidor
        client1 = new Client(1, 1, 12345, "127.0.0.1");
        client1->setClientNickName("user1");
        client1->setClientUserName("userone");
        client1->setHasValidPass(true);
        client1->setHasValidNick(true);
        client1->setHasValidUser(true);
        server->_clientsVector.push_back(client1);

        client2 = new Client(2, 2, 12346, "127.0.0.1");
        client2->setClientNickName("user2");
        client2->setClientUserName("usertwo");
        client2->setHasValidPass(true);
        client2->setHasValidNick(true);
        client2->setHasValidUser(true);
        server->_clientsVector.push_back(client2);
    }

    void TearDown() override {
        delete commandHandler;
        delete server;
        delete debug;
    }
};

#endif // TEST_HELPER_HPP