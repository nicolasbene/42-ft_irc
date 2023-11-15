#include "Server.hpp"
#include "Log.hpp"



void Server::executePart(Message message, int fd)
{
	if (message.get_parameters().empty())
		// renvoi ERR_NEEDMOREPARAM
	User client = users[fd];
	std::vector<std::string> argChannels = utils::mySplit(message.getParameters()[0], ",");
	const std::string& part_message = _message.get_parameters().size() > 1 ? _message.get_parameters()[1] : "";
	for (size_t i = 0; i < argChannels.size(); i++)
	{
		if (!isChannel(argChannels[i])) 
			// renvoi ERR_NOSUCHCHANNEL de argChannels[i]
		else 
		{
			Channel* channel = _server->get_channel(channels[i]);
			if (!client.isChannel(argChannels[i]))
				// renvoi ERR_NOTONCHANNEL de argChannels[i]
			else
				// channel->part_user(_client, part_message);
			// if (plus de user dans le channel)
				// suppression du channel
		}
	}
}