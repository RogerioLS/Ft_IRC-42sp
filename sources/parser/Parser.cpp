/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:11:00 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/server/Client.hpp"

Parser::Parser() {}

void Parser::parseBuffer(Client & client) {

	std::string buffer = client.getClientBufferChar();
	size_t pos ;

	while (true) {
		pos = buffer.find('\n');
		if (pos != std::string::npos) {
			std::string messageSplitedByLine = buffer.substr(0, pos + 1);
			client.setClientBufferStr(buffer.erase(0, pos + 1));
		}
		else
			break;
	}
}
