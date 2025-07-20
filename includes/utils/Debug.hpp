/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:47:37 by codespace         #+#    #+#             */
/*   Updated: 2025/06/22 19:13:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

# include <iostream>
# include <string>
# include <stdexcept>

<<<<<<<< HEAD:includes/utils/Debug.hpp
class Debug {
    private:
        bool _debugMode;
========
#include "./IRC.hpp"
>>>>>>>> 8ed4def (rebase ...):includes/utils/ServerInputValidation.hpp

    public:
        Debug();
        ~Debug();

<<<<<<<< HEAD:includes/utils/Debug.hpp
        void enableDebugMode(bool debugMode);
        bool isDebugMode() const;
        void debugPrint(const std::string &message, const std::string &color) const;
        static void printError(Debug &debug, const std::string &message, const std::exception &e);
========
enum ValidationStatus {
	VALIDATION_OK,
	INVALID_PORT=2,
	INVALID_PASSWORD=3,
	INVALID_BOTH=4
>>>>>>>> 8ed4def (rebase ...):includes/utils/ServerInputValidation.hpp
};

#endif