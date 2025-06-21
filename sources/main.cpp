/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:54 by codespace         #+#    #+#             */
/*   Updated: 2025/06/15 22:05:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"

int main(int argc, char **argv) {

  if (argc != 3) {
    std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
     return 1;
  }

  Server server(argv);
  try {
		server.setupServerSocket();
    server.setupEpoolEvent();
    server.setupEpoolLoop();
  } catch (const std::runtime_error& e) {
		std::cerr << "[Runtime Error] " << e.what() << std::endl;
		return 4;
	}

  return 0;
}