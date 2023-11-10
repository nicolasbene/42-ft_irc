/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:12:23 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/10 15:13:45 by nwyseur          ###   ########.fr       */
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

void Server::executeJoinOrder(const std::vector<std::string>& result, int fd)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second.getName() == result[1])
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
    addChannel(result[1], users[fd]);
    channels[result[1]].addUser(users[fd]);
    users[fd].addChannelList(channels[result[1]]);
    std::string createchan = "You created channel : [" + result[1] + "] and entered as operator";
    send(fd, createchan.c_str(), createchan.size(), 0);
}