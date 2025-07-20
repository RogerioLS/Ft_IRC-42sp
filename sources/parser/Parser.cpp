/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:09:51 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/server/Client.hpp"

Parser::Parser() {}

void Parser::parseBuffer(Client & client) {
	const std::string &buffer = client.getClientBuffer();

	std::cout << "Raw buffer: ";

	for (std::size_t i = 0; i < buffer.size(); ++i) {
		unsigned char c = buffer[i];
		if (c == '\r')
			std::cout << "\\r";
		else if (c == '\n')
			std::cout << "\\n";
		else
			std::cout << c;
}

	std::cout << std::endl;
}
