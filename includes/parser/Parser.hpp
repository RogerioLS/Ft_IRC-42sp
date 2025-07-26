/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:34 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 18:11:21 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "../utils/IRC.hpp"
#include "../utils/Debug.hpp"

class Client;

class Parser {

	private:
		Parser();

	public:


};

int		checkPortAndPassword(char **argv, Debug &debug);
bool	checkValidPassword(std::string password);
bool	checkValidPort(std::string port);
void	parseArguments(int argc, Debug &debug);

enum ValidationStatus {
	VALIDATION_OK,
	INVALID_PORT=2,
	INVALID_PASSWORD=3,
	INVALID_BOTH=4
};

#endif
