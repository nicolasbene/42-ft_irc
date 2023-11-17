/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:00:45 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/17 19:51:41 by nibenoit         ###   ########.fr       */
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
    // // Si le vecteur parameters n'est pas vide, supprime '\r\n' sur chaque parametre
    // if (!parameters.empty())
    // {
    //     for (size_t i = 0; i < parameters.size(); i++)
    //     {
    //         if (parameters[i].find("\r\n") != std::string::npos)
    //             parameters[i].erase(parameters[i].find("\r\n"), 2);
    //     }
    // }
}

Message::~Message()
{

}