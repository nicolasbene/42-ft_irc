#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Channel {
public:
	Channel(const std::string& name);
	~Channel();

	void addClient(Client* client);
	void removeClient(Client* client);
	bool hasClient(Client* client) const;
	const std::vector<Client*>& getClients() const;
	const std::string& getName() const;

private:
	std::string m_name;
	std::vector<Client*> m_clients;
};

#endif
