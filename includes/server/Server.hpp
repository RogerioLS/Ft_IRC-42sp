/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/06/27 12:14:45 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "./Client.hpp"

class Server {

	private:
		int									_port;
		std::string					_password;
		std::string 				_parsedCommand;
		int 								_serverFd;
		int									_epollFd;
		bool								_running;
		std::vector<Client> _clientsVector;
		std::vector<struct epoll_event> _eventsVector;

	public:
		Server(char **argv);
		~Server();

		void setupServer();
		void setupServerSocket();
		void setupEpollEvent();
		void setupEpollLoop();
		void setupClientVector();

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		int getServerFd() const;
		const std::string& getParsedCommand() const;
		int getEpollFd() const;
		bool getServerRunning() const;
		const std::vector<struct epoll_event>& getEvents() const;

		// Setters
		void setParsedCommand(const std::string& cmd);
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(bool running);

		template<typename T>
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

};

#endif
