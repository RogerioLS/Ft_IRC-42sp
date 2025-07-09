/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:34 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 12:48:59 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "../utils/IRC.hpp"

class Client;

class Parser {

	private:
		Parser();

	public:
		static void appendParsedCommand(Client & client);
		static void appendLineCommand(const std::string & messageSplitedByLine, Client & client);
		static std::vector<std::string> splitCommand(const std::string &command);
};

#endif
