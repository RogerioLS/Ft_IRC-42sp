/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:54 by codespace         #+#    #+#             */
/*   Updated: 2025/07/21 12:40:38 by pmelo-ca         ###   ########.fr       */
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

    std::cout << "TEST 1: [+o] " << Parser::formatOperatorModeArgs("+o") << std::endl;
    std::cout << "TEST 2: [+o+i] " << Parser::formatOperatorModeArgs("+o+i") << std::endl;
    std::cout << "TEST 3: [+-o+i] " << Parser::formatOperatorModeArgs("+-o+i") << std::endl;
    std::cout << "TEST 4: [++++-ia)+i] " << Parser::formatOperatorModeArgs("++++-ia)+i") << std::endl;
    std::cout << "TEST 5: [+o+++i+l] " << Parser::formatOperatorModeArgs("+o+++i+l") << std::endl;
    // std::cout << "TEST 6: [+o+i] " << Parser::formatOperatorModeArgs("+o+i") << std::endl;
    // std::cout << "TEST 7: [+o+i] " << Parser::formatOperatorModeArgs("+o+i") << std::endl;
    // std::cout << "TEST 8: [+o+i] " << Parser::formatOperatorModeArgs("+o+i") << std::endl;
    // std::cout << "TEST 9: [+o+i] " << Parser::formatOperatorModeArgs("+o+i") << std::endl;
    // std::cout << "TEST 10: [+o+i] " << Parser::formatOperatorModeArgs("+o+i") << std::endl;

	try {
		// Server server(argv);
		// server.setupServer();
	} catch (const std::runtime_error& e) {
		std::cerr << RED << "[Runtime Error] " << e.what() << RESET << std::endl;
		return 4;
	}
  return (0);
}
