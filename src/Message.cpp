/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:00:45 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/17 14:45:16 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(const std::string& raw) : rawMessage(raw)
{
    std::istringstream stream(rawMessage);
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
			std::getline(tmp, trailing, '\n');
        	trailing = trailing.substr(1);
			break ;
		}
        parameters.push_back(parametre);
		std::getline(tmp, parametre, ' ');
    }

    // Si le vecteur parameters n'est pas vide, remplace '\r\n' par '\0' dans le dernier élément
    if (!parameters.empty())
    {
        size_t lastIdx = parameters.size() - 1;
        size_t found = parameters[lastIdx].find("\r\n");
        if (found != std::string::npos)
        {
            parameters[lastIdx].replace(found, 2, "\0");
        }
    }
}

Message::~Message()
{

}