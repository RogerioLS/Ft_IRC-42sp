/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:26:58 by codespace         #+#    #+#             */
/*   Updated: 2025/06/22 17:30:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
# define COLORS_HPP

<<<<<<< HEAD:includes/utils/Colors.hpp
# define RESET   "\033[0m"
# define RED    "\033[1;31m"
# define GREEN  "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE   "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN   "\033[0;36m"
# define WHITE  "\033[1;37m"
# define BOLD   "\033[1m"
=======
#define MAX_PASSWORD_LENGTH 32

#include "../../utils/IRC.hpp"

int   checkPortAndPassword(char **argv);
bool  checkValidPassword(std::string password);
bool  checkValidPort(std::string port);

enum ValidationStatus {
  VALIDATION_OK,
  INVALID_PORT=2,
  INVALID_PASSWORD=3,
  INVALID_BOTH=4
};
>>>>>>> main:includes/server/utils/ServerInputValidation.hpp

#endif
