/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgautier <jgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:00:45 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/21 16:38:54 by jgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Server.hpp"

Message::Message(const std::string& raw) : rawMessage(raw)
{
    istraling = false;
    std::istringstream stream(TrimString(rawMessage, "\n\r"));//est ce que je trim ici
    std::istringstream tmp(stream.str());//seul solution pour choper le trailing proprement
	
	if (rawMessage.empty())
		return ;
    if (rawMessage[0] == ':') 
	{
        std::getline(stream, prefixe, ' ');
        std::getline(tmp, prefixe, ' ');
        prefixe = prefixe.substr(1);
    }
    std::getline(stream, commande, ' ');
    std::getline(tmp, commande, ' ');
    std::string parametre;
    while (std::getline(stream, parametre, ' ')) 
	{
		if (parametre[0] == ':')
		{
            istraling = true;
			std::getline(tmp, trailing, '\n');
        	trailing = trailing.substr(1);
			break ;
		}
        parameters.push_back(parametre);
		std::getline(tmp, parametre, ' ');
    }
}

Message::~Message()
{

}