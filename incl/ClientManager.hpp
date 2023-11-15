#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <vector>
#include <string>
#include "Client.hpp"

class ClientManager {
public:
	void addClient(Client& client);
	void removeClient(Client& client);

	// Client& getClient(const std::string& username);
	Client& getClient(int fd);
	std::vector<Client> getAllClients();

private:
	std::vector<Client> clients;
};

#endif