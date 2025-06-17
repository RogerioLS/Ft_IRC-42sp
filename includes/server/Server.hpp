/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 19:21:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <vector>
#include <cstdlib>

class Server {

	private:
		int															_port;
		std::string											_password;
		std::string 										_parsedCommand;
		int 														_serverFd;
		int															_epollFd;
		std::vector<struct epoll_event> _events;
		// []channels;
		// []clients;

	public:
		Server(char **argv);
		~Server();

		// Epoll management
		// bool addFdToEpoll(int fd, uint32_t events);
		// bool removeFdFromEpoll(int fd);
		// void resizeEvents(size_t size);
		void setupServerSocket();
		void setupEpoll();
		void startEpoolLoop();

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		int getServerFd() const;
		int getEpollFd() const;
		const std::vector<struct epoll_event>& getEvents() const;
		const std::string& getParsedCommand() const;

		// Setters
		void setParsedCommand(const std::string& cmd);
};

#endif