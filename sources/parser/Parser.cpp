/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/06/22 19:43:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Colors.hpp"

void parseArguments(int argc, Debug &debug) {
		if (argc != 3) {
				Debug::printError(debug, "Invalid number of arguments", std::invalid_argument("Usage: ./ircserv <port> <password>"));
				throw std::invalid_argument("Usage: ./ircserv <port> <password>");
		}
		debug.debugPrint("✔️ Arguments parsed successfully", CYAN);
}

int checkPortAndPassword(char **argv, Debug &debug) {
		int status = VALIDATION_OK;

		try {
				checkValidPort(std::string(argv[1]));
				debug.debugPrint("✔️ Port validated successfully: " + std::string(argv[1]), GREEN);
		} catch (const std::exception &e) {
				Debug::printError(debug, "Port validation failed", e);
				status = INVALID_PORT;
		}

		try {
				checkValidPassword(std::string(argv[2]));
				debug.debugPrint("✔️ Password validated successfully: " + std::string(argv[2]), GREEN);
		} catch (const std::exception &e) {
				Debug::printError(debug, "Password validation failed", e);
				if (status == INVALID_PORT)
						status = INVALID_BOTH;
				else
						status = INVALID_PASSWORD;
		}

		return status;
}

bool	checkValidPort(std::string port) {
	
	for (size_t i = 0; i < port.length(); ++i) {
		
		char c = port[i];
		if (!std::isdigit(c))
			throw std::invalid_argument("Port must be a number.");
	}

	int portNum = std::atoi(port.c_str());

	if (portNum < 1024 || portNum > 65535)
		throw std::out_of_range("Port must be between 1024 and 65535.");

	return true;
}

static bool isValidChar(char c) { return (c >= 32 && c <= 126); }

bool	checkValidPassword(std::string password) {

	if (password.empty())
		throw std::invalid_argument("Password cannot be empty.");
	
	if(password.length() > MAX_PASSWORD_LENGTH) {
		
		std::stringstream ss;
		ss << "Password length bigger than " << MAX_PASSWORD_LENGTH;
		throw std::invalid_argument(ss.str());
	}
	for (size_t i = 0; i < password.length(); ++i) {

		if (!isValidChar(password[i]))
			throw std::invalid_argument("Password contains invalid character.");
	}
	return true;
}