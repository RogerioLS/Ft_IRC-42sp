/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:54 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 08:41:22 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"
#include "../includes/server/utils/ServerInputValidation.hpp"

int main(int argc, char **argv) {

	if (argc != 3) {
		std::cerr << YELLOW << "Usage: ./ircserv <port> <password>" << RESET << std::endl;
		return 1;
	}

	int status = checkPortAndPassword(argv);
	if (status != VALIDATION_OK)
		return status;

	Server server(argv);
	try {
		server.setupServer();
	} catch (const std::runtime_error& e) {
		std::cerr << RED << "[Runtime Error] " << e.what() << RESET << std::endl;
		return 4;
	}

	return 0;
}
