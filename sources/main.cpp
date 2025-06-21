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
#include "../includes/server/utils/ServerInputValidation.hpp"

int main(int argc, char **argv) {

  if (argc != 3) {
    std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
     return 1;
  }
  
  int status = checkPortAndPassword(argv);
    if (status != VALIDATION_OK)
      return status;

  Server server(argv);
  try {
		server.setupServerSocket();
  } catch (const std::runtime_error& e) {
		std::cerr << "[Runtime Error] " << e.what() << std::endl;
		return 4;
	}

  std::cout << "Server starting on port " << argv[1] << " with password " << argv[2] << std::endl;
  return 0;
}