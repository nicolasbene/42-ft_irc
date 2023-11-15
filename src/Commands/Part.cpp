#include "Server.hpp"
#include "Log.hpp"



void Server::executePart(Message message, int fd)
{
	if (message.getParameters().empty())
		// renvoi ERR_NEEDMOREPARAM
	User client = users[fd];
	std::vector<std::string> argChannels = utils::mySplit(message.getParameters()[0], ",");
	for (size_t i = 0; i < argChannels.size(); i++)
	{
		if (!isChannel(argChannels[i])) 
			// renvoi ERR_NOSUCHCHANNEL de argChannels[i]
		else 
		{
			if (!client.isChannel(argChannels[i]))
				// renvoi ERR_NOTONCHANNEL de argChannels[i]
			else
				// channel->part_user(_client, part_message);
			// if (plus de user dans le channel)
				// suppression du channel
		}
	}
}