/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:57 by codespace         #+#    #+#             */
/*   Updated: 2025/07/22 12:17:55 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser/Parser.hpp"
#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Colors.hpp"
#include "../includes/server/Client.hpp"

void parseArguments(int argc, Debug &debug) {
    if (argc != 3) {
        Debug::printError(debug, "Invalid number of arguments", std::invalid_argument("Usage: ./ircserv <port> <password>"));
        throw std::invalid_argument("Usage: ./ircserv <port> <password>");
    }
    debug.debugPrint("✔️ Arguments parsed successfully", CYAN);
}

int checkPortAndPassword(char **argv, Debug &debug) {
    int status = VALIDATION_OK;

    try {
        checkValidPort(std::string(argv[1]));
        debug.debugPrint("✔️ Port validated successfully: " + std::string(argv[1]), GREEN);
    } catch (const std::exception &e) {
        Debug::printError(debug, "Port validation failed", e);
        status = INVALID_PORT;
    }

    try {
        checkValidPassword(std::string(argv[2]));
        debug.debugPrint("✔️ Password validated successfully: " + std::string(argv[2]), GREEN);
    } catch (const std::exception &e) {
        Debug::printError(debug, "Password validation failed", e);
        if (status == INVALID_PORT)
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

void Parser::appendParsedCommand(Client & client) {

	std::string buffer = client.getClientBufferStr();
	size_t pos ;

	while (true) {
		pos = buffer.find('\n');
		if (pos != std::string::npos) {
			std::cout << BOLD << "Incomming buffer: " << '"' << buffer << '"' << RESET;
			std::string messageSplitedByLine = buffer.substr(0, pos + 1);
			client.setClientBufferStr(buffer.erase(0, pos + 1));
			std::cout << CYAN << "MSBL: " << '"' << messageSplitedByLine << '"' << RESET;
			appendLineCommand(messageSplitedByLine, client);
		} else
			break;
	}
}

void Parser::appendLineCommand(const std::string & messageSplitedByLine, Client & client) {
	std::string line = messageSplitedByLine;

	if (!line.empty() && line[line.length() - 1] == '\n')
		line = line.substr(0, line.length() - 1);

	if (!line.empty() && line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);

	if (!line.empty()){
		client.appendParsedCommand(line);
		std::cout << GREEN << "Parsed Line: " << '"' << line << '"' << RESET << std:: endl;
	}
}

std::vector<std::string> Parser::splitCommand(const std::string &command) {
	std::vector<std::string> tokens;
	std::istringstream iss(command);
	std::string token;

	while (iss >> token) {
		tokens.push_back(token);
	}

	return tokens;
}

std::vector<std::string> Parser::splitCommandArg(const std::string &argsToSplit, const std::string &charsToSplit) {
  std::vector<std::string> result;

  size_t start = 0;
  size_t end = argsToSplit.find_first_of(charsToSplit);

  while (end != std::string::npos) {
    if (end > start)
      result.push_back(argsToSplit.substr(start, end - start));
    start = end + 1;
    end = argsToSplit.find_first_of(charsToSplit, start);
  }

  if (start < argsToSplit.size())
    result.push_back(argsToSplit.substr(start));

  return result;
}

std::string Parser::formatOperatorModes(const std::string &operArgs) {
  std::string result;
  char currentSign = 0;
  std::string charsToSplit = "itklo";

  for (size_t i = 0; i < operArgs.size(); ++i) {
      char c = operArgs[i];
      if (c == '+' || c == '-') {
          currentSign = c;
      } else if (charsToSplit.find(c) != std::string::npos && currentSign != 0) {
          result += currentSign;
          result += c;
      }
  }
  return result;
}

std::vector<std::string> Parser::formatOperatorModeArgs(const std::vector<std::string> &args) {
  std::vector<std::string> result;

  for (size_t i = 3; i < args.size(); ++i)
    result.push_back(args[i]);

  return result;

}
