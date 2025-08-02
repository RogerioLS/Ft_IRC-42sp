/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:47:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/28 11:07:42 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "./IRC.hpp"

class Debug {
  private:
    bool _debugMode;

  public:
    Debug();
    ~Debug();

  void enableDebugMode(bool debugMode);
  bool isDebugMode() const;
  void debugPrint(const std::string &message, const std::string &color) const;
  static void printError(Debug &debug, const std::string &message, const std::exception &e);
};

#endif
