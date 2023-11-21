/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partb.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgautier <jgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:12:36 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/21 15:16:20 by jgautier         ###   ########.fr       */
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

void Server::partb(Message message, int fd)
{
	std::string reason = "Prout";
	std::string channel = message.getParameters()[0];

	size_t posr = channel.find('\r');
    if(posr != std::string::npos)
	{
		channel = channel.erase(posr, std::string::npos);
	}

    size_t posn = channel.find('\n');
    if(posn != std::string::npos)
	{
		channel = channel.erase(posn, std::string::npos);
	}
	
	// sendServerRpl(fd, RPL_PART(users[fd].getUserNickName(), argChannels[i].substr(1)));

	// sendServerRpl(fd, RPL_PART(user_id(users[fd].getUserNickName(), users[fd].getUserName()), channel, reason));
}
