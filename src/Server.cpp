/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/31 00:15:11 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <arpa/inet.h>
#include <netdb.h>

#include <vector>
#include <poll.h> // Inclure la bibliothèque pour utiliser poll()
#include <errno.h> // Inclure la bibliothèque pour la gestion des erreurs

#include <ctime>
#include <sstream>
#include <stdexcept>

#include <fstream>
#include <iostream>

Server::~Server() {}

Server::Server(const char* port, const char* password) : _password(password) {
	int sock_fd = createSocket(port);

	if (listen(sock_fd, 10) == -1) {
		close(sock_fd);
		throw std::runtime_error(std::strerror(errno));
	}

	_clients.addPollFd(sock_fd);

	std::cout << "Server started and listening on port " << port << '\n';
}

int Server::createSocket(const char* port) {
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		throw std::runtime_error(std::strerror(errno));
	}

	int yes = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(sock_fd);
		throw std::runtime_error(std::strerror(errno));
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(port));
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock_fd, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
		close(sock_fd);
		throw std::runtime_error(std::strerror(errno));
	}
	return sock_fd;
}

void Server::addPollFd(int fd)
{
	pollfd newElem;
	newElem.fd = fd;
	newElem.events = POLLIN;
	newElem.revents = 0;
	_fds.push_back(newElem);
}
int Server::poll()
{
    // Étape 1 : Obtenez la liste des descripteurs de fichiers à surveiller
    std::vector<pollfd>& pfds = _clients.getPollfds();

    // Étape 2 : Utilisez la fonction poll pour attendre des événements sur les descripteurs de fichiers
    if (::poll(pfds.data(), pfds.size(), 1000) == -1)
    {
        // Gestion des erreurs en cas d'échec de la fonction poll
        // if (errno != EINTR)
        //     Log::error() << "poll(): " << std::strerror(errno) << '\n';

        return 1; // Code d'erreur
    }

    // Étape 3 : Parcourez la liste des descripteurs surveillés
    for (size_t i = 0; i < pfds.size(); ++i)
    {
        // Étape 4 : Vérifiez si un événement d'entrée est prêt à être lu
        if (pfds[i].revents & POLLIN)
        {
            // Étape 5 : Gestion des nouvelles connexions entrantes
            if (pfds[i].fd == pfds[0].fd)
            {
                sockaddr_storage addr;
                socklen_t addrlen = sizeof(addr);

                const int newFd = accept(pfds[i].fd, (sockaddr*)&addr, &addrlen);
                if (newFd == -1)
                {
                    // Log::error() << "accept(): " << std::strerror(errno) << '\n';
                    return 2; // Code d'erreur en cas d'échec de l'acceptation de la connexion
                }

                void* inAddr = NULL;
                if (addr.ss_family == AF_INET)
                    inAddr = &((sockaddr_in*)&addr)->sin_addr;
                else
                    inAddr = &((sockaddr_in6*)&addr)->sin6_addr;

                char IPStr[INET_ADDRSTRLEN];
                inet_ntop(addr.ss_family, inAddr, IPStr, sizeof(IPStr));

                // Log::info() << "New connection on fd " << newFd << " from " << IPStr
                //             << '\n';

                _clients.addPollFd(newFd);
                // _clients.get(newFd).second.setHost(IPStr);

                // // Si l'utilisateur est le premier à rejoindre le serveur, faites-en un opérateur réseau
                // if (_clients.size() == 1)
                //     _clients.get(newFd).second.isNetworkOperator = true;
            }
            // else
            // {
            //     // Étape 6 : Gestion de la communication avec les clients existants
            //     Client& client = _clients.get(pfds[i].fd).second;
            //     std::string packet = client.receive(pfds[i].fd, _clients, m_channels);
            //     while (!packet.empty())
            //     {
            //         const Message message(packet);
            //         logReceivedMessage(message, pfds[i].fd);

            //         const int code = Exec::exec(
            //             message, _clients, pfds[i].fd, m_channels, _password
            //         );

            //         if (code == -2)
            //             break;

            //         packet = client.receive(pfds[i].fd, _clients, m_channels, false);
            //     }
            // }
        }
    }

    return 0; // Succès
}

std::vector<pollfd>& Server::getPollfds(void)
{
	return (_fds);
}

void Server::stop()
{
	// Log::info() << "Server stopped\n";
}
