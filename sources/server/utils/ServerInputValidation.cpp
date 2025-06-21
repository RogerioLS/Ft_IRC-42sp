/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInputValidation.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:22:05 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 19:22:07 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/utils/ServerInputValidation.hpp"

int  checkPortAndPassword(char **argv) {

		int status = VALIDATION_OK;

    try {
      checkValidPort(std::string(argv[1]));
    } catch (const std::exception& e) {
      std::cerr << "Port error: " << e.what() << std::endl;
      status = INVALID_PORT;
    }

    try {
      checkValidPassword(std::string(argv[2]));
    } catch (const std::exception& e) {
      std::cerr << "Password error: " << e.what() << std::endl;
			
			if(status == INVALID_PORT)
				status = INVALID_BOTH;
			else
      	status = INVALID_PASSWORD;
    }

		return status;
}

bool  checkValidPort(std::string port) {
	
  for (size_t i = 0; i < port.length(); ++i) {
		
    char c = port[i];
    if (!std::isdigit(c))
      throw std::invalid_argument("Port must be a number.");
  }

  int portNum = std::atoi(port.c_str());

  if (portNum < 1024 || portNum > 65535)
    throw std::out_of_range("Port must be between 1024 and 65535.");

  return true;
}

static bool isValidChar(char c) { return (c >= 32 && c <= 126); }

bool  checkValidPassword(std::string password) {

  if (password.empty())
    throw std::invalid_argument("Password cannot be empty.");
	
	if(password.length() > MAX_PASSWORD_LENGTH) {
    
		std::stringstream ss;
		ss << "Password length bigger than " << MAX_PASSWORD_LENGTH;
		throw std::invalid_argument(ss.str());
	}
	for (size_t i = 0; i < password.length(); ++i) {

    if (!isValidChar(password[i]))
      throw std::invalid_argument("Password contains invalid character.");
  }
  return true;
}