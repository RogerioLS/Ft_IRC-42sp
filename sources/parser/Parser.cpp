/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 17:28:46 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/server/Client.hpp"

Parser::Parser() {}

void Parser::parseBuffer(Client & client) {
	std::cout << client.getClientBuffer() << std::endl;
}
