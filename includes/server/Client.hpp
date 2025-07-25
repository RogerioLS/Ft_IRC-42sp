/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:28 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:28:32 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./Channel.hpp"

class Client {

	private:
		int									_fd;
		int									_id;
		uint16_t						_port;
		bool								_isAuthenticated;
		std::string					_password;
		std::string					_nickName;
		std::string					_realName;
		std::string					_userName;
		std::string					_ipAddress;
		std::string					_buffer;
		bool	_hasValidPass;
		bool	_hasValidNick;
		bool	_hasValidUser;

	public:
		Client(int fd, int id);
		~Client();

		// Getters
		int getClientFd() const;
		int getClientId() const;
		bool getClientIsAuthenticated() const;
		const std::string getClientPassword() const;
		const std::string getClientNickName() const;
		const std::string getClientRealName() const;
		const std::string getClientUserName() const;
		const std::string getClientipAddress() const;
		const std::string getClientBufferStr() const;
        std::string& getClientBufferForModify();
		bool	hasValidPass()	const;
		bool	hasValidNick()	const;
		bool	hasValidUser()	const;
		bool	isFullyRegistered()	const;

		// Setters
		void setClientFd(int fd);
		void setClientId(int id);
		void setClientIsAuthenticated(bool isAuthenticated);
		void setClientPassword(const std::string& password);
		void setClientNickName(const std::string& nickName);
		void setClientRealName(const std::string& realName);
		void setClientUserName(const std::string& userName);
		void setClientIpAddress(const std::string & ipAddress);
		void setClientBufferStr(const std::string & bufferStr);
		void	setHasValidPass(bool hasPass);
		void	setHasValidNick(bool hasNick);
		void	setHasValidUser(bool hasUser);


		void appendClientBuffer(const std::string &data);
};

#endif
