/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:28 by codespace         #+#    #+#             */
/*   Updated: 2025/06/28 13:12:39 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../utils/IRC.hpp"

class Client {

	private:
		int									_fd;
		int									_id;
		bool								_isAuthenticated;
		std::string					_password;
		std::string					_nickName;
		std::string					_realName;
		std::string					_userName;
		std::string					_hostName;
		// std::vector<Channel> _clientChannelsVector;

	public:
		Client(int fd, int id);
		~Client();

		// Getters
		int getClientFd() const;
		int getClientId() const;
		bool getClientIsAuthenticated() const;
		std::string getClientPassword() const;
		std::string getClientNickName() const;
		std::string getClientRealName() const;
		std::string getClientUserName() const;
		std::string getClientHostName() const;
		// std::vector<Channel> _getClientChannels () const;

		// Setters
		void setClientFd(int fd);
		void setClientId(int id);
		void setClientIsAuthenticated(bool isAuthenticated);
		void setClientPassword(std::string password);
		void setClientNickName(std::string nickName);
		void setClientRealName(std::string realName);
		void setClientUserName(std::string userName);
		void setClientHostName(std::string hostName);
		// void addChannelToClient(std::vector<Channel>);
};

#endif
