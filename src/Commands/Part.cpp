#include "Server.hpp"
#include "Log.hpp"

void Server::executePart(Message message, int fd)
{
	if (message.get_parameters().empty())
		return reply(ERR_NEEDMOREPARAMS("PART"), 461);
	std::vector<std::string> channels = utils::split(message.getParameters()[0], ",");
	const std::string& part_message = _message.get_parameters().size() > 1 ? _message.get_parameters()[1] : "";
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!_server->channel_exists(channels[i])) 
		{
			reply(ERR_NOSUCHCHANNEL(channels[i]), 403);
		} 
		else 
		{
			Channel* channel = _server->get_channel(channels[i]);
			if (!channel->is_in_channel(_client))
				reply(ERR_NOTONCHANNEL(channels[i]), 442);
			else
				channel->part_user(_client, part_message);
			if (channel->is_empty())
				_server->delete_channel(channel->get_name());
		}
	}
	return 0;
}