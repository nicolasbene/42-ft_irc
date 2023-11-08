#include "../incl/Channel.hpp"

Channel::Channel(const std::string &name,  Client& operator)
{
    m_name = name;
    m_operator.push_back(operator);
    m_clients.push_back(operator);
}

void Channel::addClient(Client& client)
{
    m_clients.push_back(client);
}

