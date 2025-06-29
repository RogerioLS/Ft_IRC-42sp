/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:54 by codespace         #+#    #+#             */
/*   Updated: 2025/06/22 19:45:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/Server.hpp"
#include "../includes/parser/Parser.hpp"
#include "../includes/utils/Debug.hpp"
#include "../includes/utils/Colors.hpp"

int main(int argc, char **argv) {

    Debug debug;
    debug.enableDebugMode(true);

    try {
        parseArguments(argc, debug);
        int status = checkPortAndPassword(argv, debug);
        if (status != VALIDATION_OK)
            return status;
    } catch (const std::exception &e) {
        Debug::printError(debug, "Exception caught in main", e);
        return (3);
    }

    /*Server server(argv);
    debug.debugPrint("✔️ Server instance created", GREEN);
    try {
        server.setupServerSocket();
        server.setupEpollEvent();
        server.setupEpollLoop();
    } catch (const std::runtime_error& e) {
        Debug::printError(debug, "Runtime error during server setup", e);
        return (4);
    }
    std::cout << "Server starting on port " << argv[1] << " with password " << argv[2] << std::endl;*/
    return (0);
}