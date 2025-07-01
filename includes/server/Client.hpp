/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:28 by codespace         #+#    #+#             */
/*   Updated: 2025/07/01 11:16:59 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Channel.hpp"

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
		std::set<int>				_clientChannels;

	public:
		Client(int fd, int id);
		~Client();

		// Getters
		int getClientFd() const;
		int getClientId() const;
		bool getClientIsAuthenticated() const;
		const std::string & getClientPassword() const;
		const std::string & getClientNickName() const;
		const std::string & getClientRealName() const;
		const std::string & getClientUserName() const;
		const std::string & getClientHostName() const;
		const std::set<int> & getClientChannels () const;

		// Setters
		void setClientFd(int fd);
		void setClientId(int id);
		void setClientIsAuthenticated(bool isAuthenticated);
		void setClientPassword(std::string password);
		void setClientNickName(std::string nickName);
		void setClientRealName(std::string realName);
		void setClientUserName(std::string userName);
		void setClientHostName(std::string hostName);
		void setChannelToClient(int id);
};

#endif
