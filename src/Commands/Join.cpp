/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgautier <jgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:12:23 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/13 17:03:33 by jgautier         ###   ########.fr       */
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

void Server::executeJoinOrder(Message message, int fd)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second.getName() == message.getParameters()[0])
        {
            it->second.addUser(users[fd]);
            users[fd].addChannelList(it->second);
            std::string channel = it->second.getName();
            channel.resize(channel.size() - 2);
            std::string chan = "You entered channel : [" + channel + "]";
            send(fd, chan.c_str(), chan.size(), 0);
            return;
        }
    }
    addChannel(message.getParameters()[0], users[fd]);
    channels[message.getParameters()[0]].addUser(users[fd]);
    users[fd].addChannelList(channels[message.getParameters()[0]]);
    std::string createchan = "You created channel : [" + message.getParameters()[0] + "] and entered as operator";
    send(fd, createchan.c_str(), createchan.size(), 0);
}