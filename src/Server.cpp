/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/06 18:49:23 by nibenoit         ###   ########.fr       */
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
    socket_serveur(-1), _nb_clients(0), _port(port),
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
    close(socket_serveur);
    Log::info() << "Server stopped" << '\n';
}

int Server::start()
{
    // Création, liaison et écoute du socket du serveur
    socket_serveur = socket(_servinfo->ai_family, _servinfo->ai_socktype,
        _servinfo->ai_protocol);
    if (socket_serveur == -1) {
        Log::error() << "Could not create server socket" << '\n';
        exit(1);
    }
    if (bind(socket_serveur, _servinfo->ai_addr, _servinfo->ai_addrlen) == -1) {
        Log::error() << "Could not bind server socket" << '\n';
        exit(1);
    }
    if (listen(socket_serveur, MAX_CONNEXIONS) == -1) {
        Log::error() << "Could not listen on server socket" << '\n';
        exit(1);
    }
    Log::info() << "Server started" << '\n';

    return (0);
}

int Server::poll()
{
    // Crée une structure pour le socket du serveur
    epoll_event server_event;
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        Log::error() << "Could not create epoll fd" << '\n';
        exit(1);
    }
    server_event.events = EPOLLIN;
    server_event.data.fd = socket_serveur;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socket_serveur, &server_event) == -1) {
        Log::error() << "Could not add server fd to epoll" << '\n';
        exit(1);
    }

    while (true) {
        // Ne pas réutiliser num_events, car vous avez déjà une variable i dans la boucle for
        int num_events = epoll_wait(_epoll_fd, _events, MAX_CONNEXIONS, 200);
        if (num_events == -1) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < num_events; i++) {
            int fd = _events[i].data.fd;
            if (fd == socket_serveur) {
                create_client();
            } else {
                receive_message(fd);
            }
        }
    }
}

int Server::create_client()
{
    if (_nb_clients < MAX_CONNEXIONS)
    {
        struct sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_fd = accept(socket_serveur, (struct sockaddr *)&client_addr, &client_addr_size);

        if (client_fd == -1)
        {
            Log::error() << "Could not accept the client connection" << '\n';
            exit(1);
        }

        // Vous devez créer une nouvelle structure epoll_event pour le nouveau client.
        struct epoll_event client_event;
        client_event.data.fd = client_fd;
        client_event.events = EPOLLIN;

        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1)
        {
            perror("epoll_ctl");
            exit(1);
        }

        ++_nb_clients;
        Log::info() << "Client connected : " << client_fd << '\n';

        // Envoi du code RPL 001 au client
        std::string rpl001 = "001 :Welcome to the Internet Relay Network\r\n";
        send(client_fd, rpl001.c_str(), rpl001.size(), 0);
    }
    else
    {
        Log::info() << "Client connection refused: Too many clients" << '\n';
    }
    return 0;
}

int Server::receive_message(int fd)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Lire les données du socket
    int num_bytes = recv(fd, buffer, sizeof(buffer), 0);

    if (num_bytes == -1)
    {
        perror("recv");
        exit(1);
    }

    if (num_bytes == 0)
    {
        // Le client s'est déconnecté, vous devez supprimer le descripteur de fichier de epoll.
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
        {
            perror("epoll_ctl");
            exit(1);
        }

        close(fd);
        Log::info() << "Client disconnected" << '\n';
    }
    else
    {
        std::cout << "Received: " << buffer << std::endl;
    }

    return 0;
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