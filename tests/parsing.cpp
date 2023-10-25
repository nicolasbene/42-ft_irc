/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 22:54:00 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/25 22:58:48 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "tests.h"

int parsing_test_parameters(void)
{
	std::vector<std::string> tested_parameters;
	tested_parameters.push_back("PRIVMSG * LIST :");
	tested_parameters.push_back("PRIVMSG * LS :multi-prefix sasl");
	tested_parameters.push_back("PRIVMSG :bla bla bla");
	tested_parameters.push_back("PRIVMSG #chan :Hey!");
	tested_parameters.push_back("PRIVMSG #chan Hey!");
	tested_parameters.push_back("PRIVMSG #chan ::-)");
	tested_parameters.push_back("PRIVMSG ");
	tested_parameters.push_back("PRIVMSG :");
	tested_parameters.push_back("PRIVMSG bla bla bla");
	for (std::vector<std::string>::iterator it = tested_parameters.begin(); it != tested_parameters.end(); it++)
	{
		std::cout << "Testing: " << *it << std::endl;
	}
	return 0;
}
