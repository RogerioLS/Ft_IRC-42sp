/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:54 by codespace         #+#    #+#             */
/*   Updated: 2025/07/08 11:21:20 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"
#include "../includes/parser/Parser.hpp"
#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Colors.hpp"

int main(int argc, char **argv) {

	Debug debug;
	debug.enableDebugMode(true);

	try {
		parseArguments(argc, debug);
		int status = checkPortAndPassword(argv, debug);
		if (status != VALIDATION_OK)
			return status;
	} catch (const std::exception &e) {
		Debug::printError(debug, "Exception caught in main", e);
		return (3);
	}

	try {
		Server server(argv, debug);
		server.setupServer();
		server.startServerLoop();
	} catch (const std::runtime_error& e) {
		std::cerr << RED << "[Runtime Error] " << e.what() << RESET << std::endl;
		return 4;
	}
	return (0);
}
