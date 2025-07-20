/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:54 by codespace         #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/20 17:26:36 by ecoelho-         ###   ########.fr       */
=======
/*   Updated: 2025/07/03 08:41:22 by pmelo-ca         ###   ########.fr       */
>>>>>>> b39bcee (fix: :bug: Refactor Client class buffer handling; rename buffer variables for clarity and update buffer management in Server class.)
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"
#include "../includes/parser/Parser.hpp"
#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Colors.hpp"

int main(int argc, char **argv) {

  Debug debug;
  debug.enableDebugMode(true);

<<<<<<< HEAD
	try {
		parseArguments(argc, debug);
		int status = checkPortAndPassword(argv, debug);
		if (status != VALIDATION_OK)
			return status;
	} catch (const std::exception &e) {
		Debug::printError(debug, "Exception caught in main", e);
		return (3);
	}
=======
	int status = checkPortAndPassword(argv);
	if (status != VALIDATION_OK)
		return status;
>>>>>>> b39bcee (fix: :bug: Refactor Client class buffer handling; rename buffer variables for clarity and update buffer management in Server class.)

	try {
		Server server(argv);
		server.setupServer();
	} catch (const std::runtime_error& e) {
		std::cerr << RED << "[Runtime Error] " << e.what() << RESET << std::endl;
		return 4;
	}
  return (0);
}
