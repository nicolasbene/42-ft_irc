/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:16:29 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/13 17:28:07 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <poll.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Log.hpp"

std::vector<std::string> mySplit(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string extractNextWord(const std::string& input, const std::string& keyword) 
{
    std::istringstream iss(input);
    std::string word;

    // Recherche du mot-clé
    while (iss >> word) {
        if (word == keyword) {
            // Si le mot-clé est trouvé, renvoyer le prochain mot
            if (iss >> word) {
                return word;
            } else {
                // Gérer le cas où il n'y a pas de mot après le mot-clé
                return "Aucun mot après le mot-clé";
            }
        }
    }

    // Gérer le cas où le mot-clé n'est pas trouvé
    return "Mot-clé non trouvé";
}
