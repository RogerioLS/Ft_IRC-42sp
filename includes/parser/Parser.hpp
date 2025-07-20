/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:34 by codespace         #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/20 17:26:10 by ecoelho-         ###   ########.fr       */
=======
/*   Updated: 2025/07/03 12:48:59 by pmelo-ca         ###   ########.fr       */
>>>>>>> 1cbd87a (fix: :bug: Update Parser and Client classes; rename parseBuffer to appendParsedCommand, add appendLineCommand method, and adjust buffer handling in Server class.)
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
