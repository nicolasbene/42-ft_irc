
// FILEPATH: /mnt/nfs/homes/nibenoit/Documents/ft_irc/src/ClientManager.cpp
#include "ClientManager.hpp"


void ClientManager::addClient(Client& client) {
    clients.push_back(client);
}

// void ClientManager::removeClient(Client& client) {
//     for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
//         if (*it == client) {
//             clients.erase(it);
//             break;
//         }
//     }
// }

// 	Client& ClientManager::getClient(const std::string& username) {
//     for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
//         if (it->getUsername() == username) {
//             return *it;
//         }
//     }
//     return *clients.end();
// }


Client& ClientManager::getClient(int fd) {
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getFd() == fd) {
            return *it;
        }
    }
    return *clients.end();
}

// std::vector<Client> ClientManager::getAllClients() const {
//     return clients;
// }
