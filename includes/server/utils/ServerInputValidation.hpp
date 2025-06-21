/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInputValidation.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:41 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 19:21:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_INPUT_VALIDATION_HPP
#define SERVER_INPUT_VALIDATION_HPP

#define MAX_PASSWORD_LENGTH 32

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <sstream>

int   checkPortAndPassword(char **argv);
bool  checkValidPassword(std::string password);
bool  checkValidPort(std::string port);

enum ValidationStatus {
  VALIDATION_OK,
  INVALID_PORT=2,
  INVALID_PASSWORD=3,
  INVALID_BOTH=4
};

#endif