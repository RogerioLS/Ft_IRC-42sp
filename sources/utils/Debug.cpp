/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:47:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/28 11:59:57 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Colors.hpp"

Debug::Debug() : _debugMode(false) {}

Debug::~Debug() {}

void Debug::enableDebugMode(bool debugMode) {
  _debugMode = debugMode;
  debugPrint("Debug mode enabled", GREEN);
}

bool Debug::isDebugMode() const {
  return _debugMode;
}

void Debug::debugPrint(const std::string &message, const std::string &color) const {
  if (_debugMode) {
    std::cout << color << message << RESET << std::endl;
  }
}

void Debug::printError(Debug &debug, const std::string &message, const std::exception &e) {
  std::cerr << "[Error] " << message << ": " << e.what() << std::endl;
  if (debug.isDebugMode()) {
    debug.debugPrint("❌ " + message + ": " + e.what(), RED);
  }
}
