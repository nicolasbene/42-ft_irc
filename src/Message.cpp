#include "../incl/Message.hpp"

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
}

Message::~Message()
{

}