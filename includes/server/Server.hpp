/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:37 by codespace         #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/20 17:28:39 by ecoelho-         ###   ########.fr       */
=======
/*   Updated: 2025/07/04 13:00:42 by pmelo-ca         ###   ########.fr       */
>>>>>>> cf65712 (fix: :bug: Update .gitignore to include build directory and ircserv; update timestamps in Server.hpp, Parser.cpp, and Server.cpp for consistency.)
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
<<<<<<< HEAD
		volatile std::sig_atomic_t _gSignalStatus;
=======
		bool										_running;
		int											_idCounter;
		std::vector<Client>			_clientsVector;
>>>>>>> cf65712 (fix: :bug: Update .gitignore to include build directory and ircserv; update timestamps in Server.hpp, Parser.cpp, and Server.cpp for consistency.)
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
<<<<<<< HEAD
		int getServerRunning() const;
		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(int gSignalStatus);
=======
		bool getServerRunning() const;
		int getServerIdCounter() const;
		// Setters
		void setServerFd(int serverFd);
		void setEpollFd(int epollFd);
		void setServerRunning(bool running);
		void setServerIdCounter(int idCounter);
>>>>>>> cf65712 (fix: :bug: Update .gitignore to include build directory and ircserv; update timestamps in Server.hpp, Parser.cpp, and Server.cpp for consistency.)

		std::vector<Client>::iterator clientItFromFd(int fd);
		template<typename T>
		void resizeVector(std::size_t currSize, std::vector<T>& vectorToResize);

};

#endif
