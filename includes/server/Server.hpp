/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/08 11:22:08 by pmelo-ca         ###   ########.fr       */
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
		bool										_running;
		std::vector<struct epoll_event> _eventsVector;
		std::vector<Client>			_clientsVector;
		std::vector<Channel>		_channelsVector;

	public:
		static Server*					instance;

		Server(char **argv);
		~Server();

		void handleSignal();
		static void handleSigInt(int signum);
		void setupServer();
		void setupServerSocket();
		void setupEpollEvent();
		void setupEpollLoop();
		void setupClientVector();

		void handleNewClient();
		void handleClientRequest(int fd);

		void closeFds();

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		int getServerFd() const;
		int getEpollFd() const;
		int getClientCount() const;
		bool getServerRunning() const;
		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(bool running);

		std::vector<Client>::iterator clientItFromFd(int fd);
		template<typename T>
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

};

#endif
