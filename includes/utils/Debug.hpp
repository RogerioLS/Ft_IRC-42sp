/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:47:37 by codespace         #+#    #+#             */
/*   Updated: 2025/07/20 17:50:10 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

# include <iostream>
# include <string>
# include <stdexcept>

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