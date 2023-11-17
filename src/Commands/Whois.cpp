/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:09:45 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/17 14:53:49 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include "Server.hpp"
#include "Numerical_reply.hpp"

void Server::handleWhoisCommand(Message message, int fd)
{
    User *sender = getUserbyId(fd);
    std::string senderNick = sender->getUserNickName();
    
    if (message.getParameters().size() != 1)
    {
        sendToClient(fd, ERR_NEEDMOREPARAMS(senderNick, message.getCommande()));
        return ;
    }
    std::string targetNick = message.getParameters()[0];

    std::string targetUsername = "nibenoit";

    std::string hostname = "*";
    std::string realname = "Nicolas BENOIT";

    std::cout << "WHOIS " << senderNick << std::endl; //bene
    std::cout << "WHOIS " << targetNick << std::endl;
    std::cout << "WHOIS " << targetUsername << std::endl;
    
    
    sendToClient(fd, RPL_WHOISUSER(senderNick, targetNick, targetUsername, hostname, realname));
    sendToClient(fd, RPL_WHOISERVER(senderNick, targetNick, "ircserv", "ircsrv by jgautier, nibenoit, nwyseur"));
    sendToClient(fd, RPL_ENDOFWHOIS(senderNick, targetNick));
}
