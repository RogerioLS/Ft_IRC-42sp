/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:43:03 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "./Client.hpp"
#include "./Channel.hpp"
#include "../parser/Parser.hpp"
#include "../utils/Debug.hpp"
#include "../command/CommandHandler.hpp"
#include "IServer.hpp"

class CommandHandler;

class Server : public IServer {

	private:
		int											_port;
		std::string							_password;
		std::string							_serverName;
		int 										_serverFd;
		int											_epollFd;
		volatile std::sig_atomic_t _gSignalStatus;
		std::vector<struct epoll_event> _eventsVector;
		std::vector<Client>			_clientsVector;
		std::vector<Channel>		_channelsVector;
		Debug&									_debug;
		CommandHandler*					_commandHandler;

		void handleSignal();
		static void handleSigInt(int signum);

		void setupServerSocket();
		void setupEpollEvent();
		void setupEpollLoop();
		void setupClientVector();

		void handleNewClient();
		void handleClientRequest(int fd);

		void closeFds();

	public:
		Server(char **argv, Debug& debug);
		~Server();

		void setupServer();
		void startServerLoop();

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		const std::string& getServerName() const;
		int getServerFd() const;
		int getEpollFd() const;
		int getClientCount() const;
		int getServerRunning() const;
		Debug& getDebug();
		Channel* getChannelByName(const std::string& name);
		Client* getClientByNickname(const std::string& nickname);
		Client* getClientById(int id);
		const std::vector<Channel>& getChannels() const;

		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(int gSignalStatus);

		void createChannel(const std::string& name, Client& client);
		void addClientForTest(Client* client);
		void sendMessage(int fd, const std::string& message);

		std::vector<Client>::iterator clientItFromFd(int fd);
		template<typename T>
<<<<<<< HEAD
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);
		std::vector<Client> &getClientsVector();
		void sendMessage(int fd, const std::string& message);

};

#endif
