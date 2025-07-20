/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:11:41 by ecoelho-         ###   ########.fr       */
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
			std::string messageSplitedByLine = buffer.substr(0, pos + 1);
			client.setClientBufferStr(buffer.erase(0, pos + 1));
			appendLineCommand(messageSplitedByLine, client);
		}
		else
			break;
	}
}

void Parser::appendLineCommand(const std::string & messageSplitedByLine, Client & client) {
	std::string line = messageSplitedByLine;

	if (!line.empty() && line[line.length() - 1] == '\n')
		line = line.substr(0, line.length() - 1);

	if (!line.empty() && line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);

	if (!line.empty())
		client.appendParsedCommand(line);
}
