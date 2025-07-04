/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 13:00:42 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../utils/IRC.hpp"

class Client;

class Server {

	private:
		int											_port;
		std::string							_password;
		int 										_serverFd;
		int											_epollFd;
		bool										_running;
		int											_idCounter;
		std::vector<Client>			_clientsVector;
		std::vector<struct epoll_event> _eventsVector;

	public:
		Server(char **argv);
		~Server();

		void setupServer();
		void setupServerSocket();
		void setupEpollEvent();
		void setupEpollLoop();
		void setupClientVector();

		void handleNewClient();
		void handleClientRequest(int fd);

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		int getServerFd() const;
		int getEpollFd() const;
		int getClientCount() const;
		bool getServerRunning() const;
		int getServerIdCounter() const;
		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(bool running);
		void setServerIdCounter(int idCounter);

		std::vector<Client>::iterator clientItFromFd(int fd);
		template<typename T>
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

};

#endif
