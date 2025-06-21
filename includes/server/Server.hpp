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
 
#include "../utils/IRC.hpp"

class Server {

	private:
		int															_port;
		std::string											_password;
		std::string 										_parsedCommand;
		int 														_serverFd;
		int															_epollFd;
		struct epoll_event							_eventStruct;
		std::vector<struct epoll_event> _eventsVector;

	public:
		Server(char **argv);
		~Server();

		void setupServerSocket();
		void setupEpollEvent();
		void setupEpollLoop();

		// Getters
		int getPort() const;
		const std::string& getPassword() const;
		int getServerFd() const;
		const std::string& getParsedCommand() const;
		int getEpollFd() const;
		struct epoll_event getEventStruct() const;
		const std::vector<struct epoll_event>& getEvents() const;

		// Setters
		void setParsedCommand(const std::string& cmd);
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setEventStruct(epoll_event eventStruct);
};

#endif