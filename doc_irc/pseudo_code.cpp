#include "pseudo_code.hpp"

void	User::whoIs(const User &target) const
{
	std::cout << target.getFullname()<< std::endl;
}