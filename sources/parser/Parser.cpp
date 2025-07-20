/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
<<<<<<< HEAD
<<<<<<< HEAD
/*   Updated: 2025/07/20 17:30:04 by ecoelho-         ###   ########.fr       */
=======
/*   Updated: 2025/07/03 13:05:55 by pmelo-ca         ###   ########.fr       */
>>>>>>> 1cbd87a (fix: :bug: Update Parser and Client classes; rename parseBuffer to appendParsedCommand, add appendLineCommand method, and adjust buffer handling in Server class.)
=======
/*   Updated: 2025/07/04 13:16:47 by pmelo-ca         ###   ########.fr       */
>>>>>>> cf65712 (fix: :bug: Update .gitignore to include build directory and ircserv; update timestamps in Server.hpp, Parser.cpp, and Server.cpp for consistency.)
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/server/Client.hpp"

Parser::Parser() {}

<<<<<<< HEAD
void Parser::parseBuffer(Client & client) {
	std::cout << client.getClientBuffer() << std::endl;
=======
void Parser::appendParsedCommand(Client & client) {

	std::string buffer = client.getClientBufferChar();
	size_t pos ;

	while (true) {
		pos = buffer.find('\n');
		if (pos != std::string::npos) {
			std::cout << BOLD << "Incomming buffer: " << '"' << buffer << '"' << RESET;
			std::string messageSplitedByLine = buffer.substr(0, pos + 1);
			client.setClientBufferStr(buffer.erase(0, pos + 1));
			std::cout << CYAN << "MSBL: " << '"' << messageSplitedByLine << '"' << RESET;
			appendLineCommand(messageSplitedByLine, client);
		} else
			break;
	}
>>>>>>> 1cbd87a (fix: :bug: Update Parser and Client classes; rename parseBuffer to appendParsedCommand, add appendLineCommand method, and adjust buffer handling in Server class.)
}

void Parser::appendLineCommand(const std::string & messageSplitedByLine, Client & client) {
	std::string line = messageSplitedByLine;

	if (!line.empty() && line[line.length() - 1] == '\n')
		line = line.substr(0, line.length() - 1);

	if (!line.empty() && line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);

	if (!line.empty()){
		client.appendParsedCommand(line);
		std::cout << GREEN << "Parsed Line: " << '"' << line << '"' << RESET << std:: endl;
	}
}

std::vector<std::string> Parser::splitCommand(const std::string &command) {
	std::vector<std::string> tokens;
	std::istringstream iss(command);
	std::string token;

	while (iss >> token) {
		tokens.push_back(token);
	}
	
	return tokens;
}
