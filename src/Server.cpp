/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/30 16:02:37 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <arpa/inet.h>
#include <netdb.h>

#include <cerrno>
#include <ctime>
#include <sstream>
#include <stdexcept>

#include <fstream>
#include <iostream>


Server::Server(const char* port, const char* password) : m_password(password)
{
    int sock_fd = 0; // Descripteur de fichier du socket
    int yes = 1; // Variable utilisée pour configurer les options du socket
    addrinfo hints; // Structure pour spécifier les critères de getaddrinfo

    // Initialise la structure hints à zéro et spécifie le type de socket et les options
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
    hints.ai_socktype = SOCK_STREAM; // Socket de type flux (TCP)
    hints.ai_flags = AI_PASSIVE; // Adresse IP locale

    addrinfo* result = NULL; // Pointeur pour stocker les informations d'adresse obtenues
    const int error = getaddrinfo(NULL, port, &hints, &result); // Obtient les informations d'adresse
    if (error)
    {
        freeaddrinfo(result); // Libère la mémoire allouée
        throw std::runtime_error(gai_strerror(error)); // Génère une exception en cas d'erreur
    }

    addrinfo* tmp = NULL; // Pointeur temporaire pour parcourir les informations d'adresse
    for (tmp = result; tmp != NULL; tmp = tmp->ai_next)
    {
        // Crée un socket en utilisant les informations d'adresse actuelles
        sock_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        if (sock_fd < 0)
            continue; // Passe à l'adresse suivante en cas d'erreur

        // Configure le socket pour éviter l'erreur "address already in use"
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))
        {
            freeaddrinfo(result); // Libère la mémoire allouée
            throw std::runtime_error(std::strerror(errno)); // Génère une exception en cas d'erreur
        }

        // Lie le socket à l'adresse actuelle
        if (bind(sock_fd, tmp->ai_addr, tmp->ai_addrlen) < 0)
        {
            close(sock_fd); // Ferme le socket en cas d'erreur
            continue; // Passe à l'adresse suivante
        }

        break;
    }

    freeaddrinfo(result); // Libère la mémoire allouée par getaddrinfo

    if (tmp == NULL)
        throw std::runtime_error("Server can't bind to any port"); // Génère une exception si le serveur ne peut pas se lier à un port

    if (listen(sock_fd, 10) == -1)
        throw std::runtime_error(std::strerror(errno)); // Génère une exception en cas d'erreur lors de la mise en écoute du socket

    // m_clients.addListener(sock_fd); // Ajoute le socket au gestionnaire de clients

    std::cout << "Server started and listening on port " << port << '\n'; // Affiche un message de démarrage

	
}

void Server::stop()
{
	// Log::info() << "Server stopped\n";
}

Server::~Server() {}

