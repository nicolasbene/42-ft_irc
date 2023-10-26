/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 22:52:24 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/26 16:34:33 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Message.hpp"
#include "constant.h"

Message::Message(void) : raw(), tags(), prefix(), parameters()
{
	this->cmd = PRIVMSG;
}

Message::Message(std::string)
{	
}

Message::Message(Message const & src) : raw(src.raw), tags(src.tags), prefix(src.prefix),
	cmd(src.cmd), parameters(src.parameters)
{
}

Message::~Message(void) {}

Message & Message::operator=(Message const & rhs)
{
	if (this != &rhs)
	{
		this->raw = rhs.raw;
		this->tags = rhs.tags;
		this->prefix = rhs.prefix;
		this->cmd = rhs.cmd;
		this->parameters = rhs.parameters;
	}
	return *this;
}
