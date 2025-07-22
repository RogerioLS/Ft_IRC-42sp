/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:34 by codespace         #+#    #+#             */
/*   Updated: 2025/07/22 12:16:42 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#define MAX_PASSWORD_LENGTH 32

#include "../utils/IRC.hpp"

class Client;

class Debug;

class Parser {

	private:
		Parser();

	public:
		static void appendParsedCommand(Client & client);
		static void appendLineCommand(const std::string & messageSplitedByLine, Client & client);
		static std::vector<std::string> splitCommand(const std::string &command);
    static std::vector<std::string> splitCommandArg(const std::string &argsToSplit, const std::string &charsToSplit);
    static std::string formatOperatorModes(const std::string &operArgs);
    static std::vector<std::string> formatOperatorModeArgs(const std::vector<std::string> &args);
  };

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
