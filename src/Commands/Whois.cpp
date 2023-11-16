/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:09:45 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/16 18:46:59 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include "Server.hpp"
#include "Numerical_reply.hpp"

void Server::handleWhoisCommand(Message message, int fd)
{
    std::string sender_nickname = users[fd].getUserNickName();
    if (message.getParameters().size() == 0)
    {
        sendToClient(fd, ERR_NONICKNAMEGIVEN(sender_nickname));
        return;
    }
    std::string target_nickname = message.getParameters()[0];
    // if (users.find(target_nickname) == users.end())
    // {
    //     sendToClient(fd, ERR_NOSUCHNICK(sender_nickname, target_nickname));
    //     return;
    // }
    
    std::string target_username = users[fd].getUserName();
    std::string target_realname = "Nicolas BENOIT";
    std::string target_hostname = "*";
    std::string target_servername = "ircserv";
    
    sendToClient(fd, RPL_WHOISUSER(sender_nickname, target_nickname, target_username, target_hostname, target_realname));
    sendToClient(fd, RPL_WHOISERVER(sender_nickname, target_servername, "ircserv", "ircsrv by jgautier, nibenoit, nwyseur"));
    sendToClient(fd, RPL_ENDOFWHOIS(sender_nickname, "End of WHOIS list"));
}
