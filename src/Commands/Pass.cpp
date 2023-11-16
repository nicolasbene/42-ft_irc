#include "Server.hpp"
#include "Log.hpp"

void Server::PassAuthentification(Message msg, int fd)
{
    if (msg.getParameters().empty())
		// renvoi ERR_NEEDMOREPARAM
    else if (users[fd].getIsConnected())
        // renvoi ERR_ALREADYREGISTRED
    else if(msg.getParameters()[0] != _password)
        // renvoi ERR_WRONGPASSWORD
    else if(msg.getParameters()[0] == _password)
    {
        users[fd].setIsConnected(true);
        //renvoi welcome dans le server
    }
}