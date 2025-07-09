/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/04 13:16:47 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/server/Client.hpp"

Parser::Parser() {}

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
