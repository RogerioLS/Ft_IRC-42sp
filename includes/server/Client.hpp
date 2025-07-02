/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:28 by codespace         #+#    #+#             */
/*   Updated: 2025/07/02 12:18:16 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../utils/IRC.hpp"
#include "../parser/Parser.hpp"

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
		std::vector<std::string> _parsedCommand;

	public:
		Client(int fd, int id, uint16_t port, std::string ipAddress);
		~Client();

		// Getters
		int getClientFd() const;
		int getClientId() const;
		uint16_t getClientPort() const;
		bool getClientIsAuthenticated() const;
		const std::string getClientPassword() const;
		const std::string getClientNickName() const;
		const std::string getClientRealName() const;
		const std::string getClientUserName() const;
		const std::string getClientipAddress() const;
		const std::string getClientBuffer() const;
		const std::vector<std::string> getClientParsedCommand() const;

		// Setters
		void setClientFd(int fd);
		void setClientId(int id);
		void setClientPort(uint16_t port);
		void setClientIsAuthenticated(bool isAuthenticated);
		void setClientPassword(const std::string& password);
		void setClientNickName(const std::string& nickName);
		void setClientRealName(const std::string& realName);
		void setClientUserName(const std::string& userName);
		void setClientIpAddress(const std::string & ipAddress);
		void setClientBuffer(const std::string & buffer);
		void setClientParsedCommand(const std::vector<std::string> & parsedCommand);
};

#endif
