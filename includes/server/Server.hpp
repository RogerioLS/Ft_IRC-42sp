/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/08 11:41:17 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "./Client.hpp"
#include "./Channel.hpp"
#include "../utils/Debug.hpp"
#include "../command/CommandHandler.hpp"

class CommandHandler;

class Server {

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
		static Server*					instance;

		Server(char **argv, Debug& debug);
		~Server();

		void setupServer();

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
		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(int gSignalStatus);

		void createChannel(const std::string& name, Client& client);

		std::vector<Client>::iterator clientItFromFd(int fd);
		template<typename T>
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

};

#endif
