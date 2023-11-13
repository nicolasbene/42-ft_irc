#include "Server.hpp"
#include "Log.hpp"

void Server::executePart(Message message, int fd)
{
    std::string user_id = users[fd].getUserID();
    std::string channel = message.getParameters().front().substr(1);
    std::string trailing = ":" + message.getTrailing();
    sendServerRpl(fd, RPL_PART(user_id, channel, trailing));
}