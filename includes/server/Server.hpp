/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:17:59 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "./Client.hpp"
#include "./Channel.hpp"

class Server {

	private:
		int											_port;
		std::string							_password;
		int 										_serverFd;
		int											_epollFd;
		volatile std::sig_atomic_t _gSignalStatus;
		std::vector<struct epoll_event> _eventsVector;
		std::vector<Client>			_clientsVector;
		std::vector<Channel>		_channelsVector;

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

		Server(char **argv);
		~Server();

		void setupServer();

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		int getServerFd() const;
		int getEpollFd() const;
		int getClientCount() const;
		int getServerRunning() const;
		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(int gSignalStatus);

		std::vector<Client>::iterator clientItFromFd(int fd);
		template<typename T>
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);
		std::vector<Client> &getClientsVector();

};

#endif
