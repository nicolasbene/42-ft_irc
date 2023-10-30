#include <iostream>
#include "Server.hpp"
#include <cstring>

int	main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}

	if (strlen(argv[2]) < 8)
	{
		std::cout << "Invalid password" << std::endl;
		return (1);
	}

	Server	server(argv[1], argv[2]);

	return (0);
}
