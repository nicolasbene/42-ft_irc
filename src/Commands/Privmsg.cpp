/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:12:23 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/10 18:16:49 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <poll.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Log.hpp"

void Server::sendPrivateMessage(Message message, int fd)
{
    std::string sender = users[fd].getUserNickName();
    sender.resize(sender.size() - 2);

    std::map<int, User>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
        if (it->second.getUserName() == message.getParameters()[0] + "\r\n" || it->second.getUserNickName() == message.getParameters()[0] + "\r\n")
        {
            std::string buffer = "[" + sender + "]";
            for (unsigned long int k = 2; k < message.getParameters().size(); k++)
                buffer = buffer + " " + message.getParameters()[k];
            send(it->second.getUserSockId(), buffer.c_str(), buffer.size(), 0);
            return;
        }
    }
    std::string error = "Error: the user you're trying to reach doesn't exist\r\n";
    send(fd, error.c_str(), error.size(), 0);
}
