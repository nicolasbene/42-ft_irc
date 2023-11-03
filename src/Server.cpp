/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/03 13:37:14 by nibenoit         ###   ########.fr       */
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
#include "Log.hpp"

Server::Server(const std::string& port, const std::string& password) :
    _sockfd(-1), _nb_clients(0), _port(port),
    _password(password), _name(SERVER_NAME), _hints(), _servinfo(NULL)
{
    // Affiche les informations sur le port et le mot de passe
    Log::info() << "Using port: " << port << '\n';
    Log::info() << "Using password: '" << password << "'" << '\n';

    // Initialise la structure _hints pour la fonction getaddrinfo
    memset(&_hints, 0, sizeof(_hints));
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;

    // Obtient les détails de connexion du serveur en utilisant getaddrinfo
    if (getaddrinfo(NULL, _port.c_str(), &_hints, &_servinfo) != 0) {
        Log::error() << "Could not get server connection details" << '\n';
        exit(1);
    }
}

Server::~Server()
{
    freeaddrinfo(_servinfo);
    close(_sockfd);
    Log::info() << "Server stopped" << '\n';
}

int Server::start()
{
    // Création, liaison et écoute du socket du serveur
    _sockfd = socket(_servinfo->ai_family, _servinfo->ai_socktype,
        _servinfo->ai_protocol);
    if (_sockfd == -1) {
        Log::error() << "Could not create server socket" << '\n';
        exit(1);
    }
    if (bind(_sockfd, _servinfo->ai_addr, _servinfo->ai_addrlen) == -1) {
        Log::error() << "Could not bind server socket" << '\n';
        exit(1);
    }
    if (listen(_sockfd, MAX_CONNEXIONS) == -1) {
        Log::error() << "Could not listen on server socket" << '\n';
        exit(1);
    }
    Log::info() << "Server started" << '\n';

    return (0);
}

int Server::poll()
{
    // Crée une structure pour le socket du serveur
    pollfd server_pfd;
    server_pfd.fd = _sockfd;
    server_pfd.events = POLLIN;
    server_pfd.revents = 0;

    // Crée un vecteur de structures pour les sockets clients
    
    _client_pfds.reserve(MAX_CONNEXIONS);

    // Ajoute le socket du serveur au vecteur
    _client_pfds.push_back(server_pfd);
    ++_nb_clients;

    std::cout << "Server is now waiting for connections on port: " << _port << std::endl;

    // Boucle infinie pour surveiller les sockets
    while (true) {
        if (::poll(&_client_pfds[0], _nb_clients, 200) == -1) {
            Log::error() << "Could not poll the server socket" << '\n';
            exit(1);
        }

        // Parcourt tous les sockets clients pour vérifier les événements d'entrée
        for (int i = 0; i < _nb_clients; i++) {
            if (_client_pfds[i].revents & POLLIN) {
                int fd = _client_pfds[i].fd;
                if (fd == _sockfd) {
                    create_client();
                } else {
                    receive_message(i);
                }
            }
        }
    }
}

int	Server::create_client()
{
    if (_nb_clients < MAX_CONNEXIONS) {
        struct sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_fd = accept(_sockfd, (struct sockaddr*)&client_addr, &client_addr_size);

        if (client_fd == -1) {
            Log::error() << "Could not accept the client connection" << '\n';
            exit(1);
        }

        pollfd new_client_pfd;
        new_client_pfd.fd = client_fd;
        new_client_pfd.events = POLLIN;
        new_client_pfd.revents = 0;
        _client_pfds.push_back(new_client_pfd);
        ++_nb_clients;
        Log::info() << "Client connected : " << _sockfd << '\n';

        //         // Envoi du code RPL 001 au client
        std::string rpl001 = "001 :Welcome to the Internet Relay Network\r\n";
        send(client_fd, rpl001.c_str(), rpl001.size(), 0);
        
    } else {
        Log::info() << "Client connection refused: Too many clients" << '\n';
    }
	return (0);
}

int	Server::receive_message(int i)
{
    int fd = _client_pfds[i].fd;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int num_bytes = recv(fd, buffer, sizeof(buffer), 0);

    if (num_bytes == -1) {
        Log::error() << "Could not receive data from the client" << '\n';
        exit(1);
    }

    if (num_bytes == 0) {
        close(fd);
        _client_pfds.erase(_client_pfds.begin() + i);
        --_nb_clients;
        Log::info() << "Client disconnected" << '\n';
    } else {
        std::cout << "Received: " << buffer << std::endl;
    }
	return (0);
}

bool Server::is_valid_port(const std::string& port)
{
    if (port.empty())
        return false;
    for (std::string::const_iterator it = port.begin(); it != port.end(); it++) {
        if (!isdigit(*it))
            return false;
    }
    int portNum = atoi(port.c_str());
    return portNum >= 1024 && portNum <= 65535;
}

bool Server::is_valid_password(const std::string& password)
{
    if (password.empty() || password.size() > 255)
        return false;
    for (std::string::const_iterator it = password.begin(); it != password.end(); it++) {
        if (isspace(*it) || *it == '\0' || *it == ':')
            return false;
    }
    return true;
}

void Server::write_logo() const
{
    std::ifstream infile("assets/logo.txt");
    std::string line;
    while (std::getline(infile, line))
    {
        std::cout << line << std::endl;
    }
}