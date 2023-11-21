#include "Server.hpp"

bool forbidenChar(std::string& str, )

void Server::executeKick(Message msg, int fd)
{
    if (msg.getParameters().empty() || msg.getParameters().size() < 2 || !msg.getParameters()[0].size() || !msg.getParameters()[1].size())
		sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), "KICK"));
    if (msg.getParameters()[0].find(",") != std::string::npos || msg.getParameters()[1].find(",") != std::string::npos)
		sendServerRpl(fd, ERR_BADCHANMASK(users[fd].getUserNickName(), "KICK" + " " + msg.getParameters()[0] + " " + msg.getParameters()[1]));
    if (!isChannel( msg.getParameters()[0].substr(1))) 
			sendServerRpl(fd, ERR_NOSUCHCHANNEL(users[fd].getUserNickName(), msg.getParameters()[0]))
    // if (!user[].isChannel(msg.getParameters()[0].substr(1))) pas user fd mais le user qu on cherche
				sendServerRpl(fd, ERR_NOTONCHANNEL(users[fd].getUserNickName(), msg.getParameters()[0].substr(1)));
}