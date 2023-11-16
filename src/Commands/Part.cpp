#include "Server.hpp"
#include "Log.hpp"


static void Server::unregisterClientToChannel(std::string unregisterChannnel, int fd)
{
	channels[unregisterChannnel].removeUser(users[fd]);
	// users[fd]._channelList.erase(unregisterChannnel);
	//faut il supprimer le client autre part???
}


void Server::executePart(Message message, int fd)
{
	if (message.getParameters().empty())
		sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), "PART"));
	User client = users[fd];
	std::vector<std::string> argChannels = utils::mySplit(message.getParameters()[0], ",");
	for (size_t i = 0; i < argChannels.size(); i++)
	{
		if (!isChannel(argChannels[i].substr(1))) 
			sendServerRpl(fd, ERR_NOSUCHCHANNEL(users[fd].getUserNickName(), argChannels[i].substr(1)));
		else 
		{
			if (!client.isChannel(argChannels[i].substr(1)))
				sendServerRpl(fd, ERR_NOTONCHANNEL(users[fd].getUserNickName(), argChannels[i].substr(1)));
			else
			{
				sendServerRpl(fd, RPL_PART(users[fd].getUserID(), argChannels[i].substr(1), message.getTrailing()));
				unregisterClientToChannel(argChannels[i].substr(1), fd);
			}
		}
	}
}