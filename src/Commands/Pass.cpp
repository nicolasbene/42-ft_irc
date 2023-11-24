#include "Server.hpp"
#include "Log.hpp"

void Server::PassAuthentification(Message msg, int fd)
{
  if (msg.getParameters().empty())
		return (sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), "PASS")));
  else if (!users[fd].getIsConnected())
		sendServerRpl(fd, ERR_ALREADYREGISTERED(users[fd].getUserNickName()));
  else if(msg.getParameters()[0] != _password)
		sendServerRpl(fd, ERR_PASSWDMISMATCH(users[fd].getUserNickName()));
  else if(msg.getParameters()[0] == _password)
  {
      users[fd].setIsConnected(true);
  }
}