/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 22:52:24 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/25 22:52:26 by nibenoit         ###   ########.fr       */
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
		
std::string Message::getRaw(void) const
{
	return this->raw;
}

std::map<std::string, std::string> Message::getTags(void) const
{
	return this->tags;
}

std::string Message::getPrefix(void) const
{
	return this->prefix;
}

t_cmd_type Message::getCmd(void) const
{
	return this->cmd;
}

std::vector<std::string> Message::getParameters(void) const
{
	return this->parameters;
}

t_parse_return Message::addRaw(std::string raw)
{
}

t_parse_return Message::addTag(std::string tag)
{
}

t_parse_return Message::addPrefix(std::string prefix)
{
}

t_parse_return Message::addCmd(t_cmd_type cmd)
{
}

t_parse_return Message::addParameter(std::string parameter)
{
}
