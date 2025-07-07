/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:06:27 by codespace         #+#    #+#             */
/*   Updated: 2025/06/29 20:11:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "../../includes/parser/Parser.hpp"
#include "../../includes/utils/Debug.hpp"

TEST(ParserTests, ValidArguments) {
	Debug debug;
	debug.enableDebugMode(false);
	EXPECT_NO_THROW(parseArguments(3, debug));
}

TEST(ParserTests, InvalidArgumentCount) {
	Debug debug;
	debug.enableDebugMode(false);
	EXPECT_THROW(parseArguments(2, debug), std::invalid_argument);
}

TEST(ParserTests, InvalidPort) {
	Debug debug;
	debug.enableDebugMode(false);
	char *argv[] = { (char *)"./ircserv", (char *)"invalid_port", (char *)"password" };
	EXPECT_EQ(checkPortAndPassword(argv, debug), INVALID_PORT);
}

TEST(ParserTests, InvalidPassword) {
	Debug debug;
	debug.enableDebugMode(false);
	char *argv[] = { (char *)"./ircserv", (char *)"6667", (char *)"" };
	EXPECT_EQ(checkPortAndPassword(argv, debug), INVALID_PASSWORD);
}

TEST(ParserTests, BothInvalid) {
	Debug debug;
	debug.enableDebugMode(false);
	char *argv[] = { (char *)"./ircserv", (char *)"invalid_port", (char *)"" };
	EXPECT_EQ(checkPortAndPassword(argv, debug), INVALID_BOTH);
}

TEST(ParserTests, ValidPortAndPassword) {
	Debug debug;
	debug.enableDebugMode(false);
	char *argv[] = { (char *)"./ircserv", (char *)"6667", (char *)"password" };
	EXPECT_EQ(checkPortAndPassword(argv, debug), VALIDATION_OK);
}