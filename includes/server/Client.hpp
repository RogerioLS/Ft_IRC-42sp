/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:28 by codespace         #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/20 17:31:27 by ecoelho-         ###   ########.fr       */
=======
/*   Updated: 2025/07/08 20:25:54 by ecoelho-         ###   ########.fr       */
>>>>>>> 24726f5 (Finish the register commands handles)
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
<<<<<<< HEAD
		std::string					_hostName;
		std::set<int>				_clientChannels;
=======
		std::string					_ipAddress;
		std::string					_buffer;
		char								_bufferChar[BUFFER_SIZE];
		std::vector<std::string> _parsedCommand;
		bool	_hasValidPass;
		bool	_hasValidNick;
		bool	_hasValidUser;
>>>>>>> af39712 (commands pass, nick, user)

	public:
		Client(int fd, int id);
		~Client();

		// Getters
		int getClientFd() const;
		int getClientId() const;
		bool getClientIsAuthenticated() const;
<<<<<<< HEAD
		const std::string & getClientPassword() const;
		const std::string & getClientNickName() const;
		const std::string & getClientRealName() const;
		const std::string & getClientUserName() const;
		const std::string & getClientHostName() const;
=======
		const std::string getClientPassword() const;
		const std::string getClientNickName() const;
		const std::string getClientRealName() const;
		const std::string getClientUserName() const;
		const std::string getClientipAddress() const;
		char * getClientBufferChar();
		const std::string getClientBufferStr() const;
		const std::vector<std::string> getClientParsedCommand() const;
		bool	hasValidPass()	const;
		bool	hasValidNick()	const;
		bool	hasValidUser()	const;
		bool	isFullyRegistered()	const;
>>>>>>> af39712 (commands pass, nick, user)

		// Setters
		void setClientFd(int fd);
		void setClientId(int id);
		void setClientIsAuthenticated(bool isAuthenticated);
<<<<<<< HEAD
		void setClientPassword(std::string password);
		void setClientNickName(std::string nickName);
		void setClientRealName(std::string realName);
		void setClientUserName(std::string userName);
		void setClientHostName(std::string hostName);
=======
		void setClientPassword(const std::string& password);
		void setClientNickName(const std::string& nickName);
		void setClientRealName(const std::string& realName);
		void setClientUserName(const std::string& userName);
		void setClientIpAddress(const std::string & ipAddress);
		void setClientBufferStr(const std::string & bufferStr);
		void setClientParsedCommand(const std::vector<std::string> & parsedCommand);
		void appendParsedCommand(const std::string & line);
		void	setHasValidPass(bool hasPass);
		void	setHasValidNick(bool hasNick);
		void	setHasValidUser(bool hasUser);
<<<<<<< HEAD
>>>>>>> af39712 (commands pass, nick, user)
=======

		void appendClientBuffer(const std::string &data);
		void clearParsedCommands();
>>>>>>> 24726f5 (Finish the register commands handles)
};

#endif
