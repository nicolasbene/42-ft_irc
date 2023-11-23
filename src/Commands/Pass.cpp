#include "Server.hpp"
#include "Log.hpp"

void Server::PassAuthentification(Message msg, int fd)
{
    if (msg.getParameters().empty())
		sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), "PASS"));//pb car le nick name pas encore set
    else if (users[fd].getIsConnected())
		sendServerRpl(fd, ERR_ALREADYREGISTERED(users[fd].getUserNickName()));//pb car le nick name pas encore set
    else if(msg.getParameters()[0] != _password)
		sendServerRpl(fd, ERR_PASSWDMISMATCH(users[fd].getUserNickName()));//pb car le nick name pas encore set
    else if(msg.getParameters()[0] == _password)
    {
        users[fd].setIsConnected(true);
    }
}