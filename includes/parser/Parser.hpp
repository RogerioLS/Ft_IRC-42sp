/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:34 by codespace         #+#    #+#             */
/*   Updated: 2025/06/22 19:39:08 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#define MAX_PASSWORD_LENGTH 32

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <sstream>
#include "../utils/Debug.hpp"

int   checkPortAndPassword(char **argv, Debug &debug);
bool  checkValidPassword(std::string password);
bool  checkValidPort(std::string port);
void  parseArguments(int argc, Debug &debug);

enum ValidationStatus {
  VALIDATION_OK,
  INVALID_PORT=2,
  INVALID_PASSWORD=3,
  INVALID_BOTH=4
};

#endif