/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:10:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/09 17:50:15 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
# include <iostream>
# include "Server.hpp"
# include "User.hpp"

class User;

class Channel
{
	public:
		Channel(void);
		Channel(const std::string& name, User& channelOperator);
		~Channel();
	
		void addUser(User& User);
		void removeUser(User& User);
		bool hasUser(User& User) const;
		const std::vector<User*> getUsers() const;
		const std::string getName() const;


	private:
		std::string _channelName;
		std::string _channelTopic;
		std::vector<User*> _channelMembers;
		std::vector<User*> _channelOperators;
		int _usercount;
		int _userlimit;
};

#endif