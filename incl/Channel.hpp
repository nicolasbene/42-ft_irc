/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:10:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/20 15:05:43 by nwyseur          ###   ########.fr       */
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
	
		void addUser(User& user);
		void removeUser(User& user);
		void addBannedUser(User& user);
		void removeBannedUser(User& user);
		void addKickedUser(User& user);
		void removeKickedUser(User& user);
		bool hasUser(User& user) const;

		const std::vector<User*> getChannelMembers() const;
		const std::vector<User*> getBannedUsers() const;
		const std::vector<User*> getKickedUsers() const;
		const std::vector<User*> getChannelOperators() const;
		const std::string getName() const;
		const std::string getPassword() const;
		size_t getChannelCap() const;
		const std::string getSymbol() const;
		const std::string getChannelTopic() const;
		

		void	setPassword(std::string pass);
		void	setChannelCap(int cap);
		void	setChannelTopic(std::string topic);
		void	setChannelOperator(User& ops);
		void	setBannedUser(User& userb);
		void	setKickedUser(User& userk);

		std::string listOfMember() const;


	private:
		std::string _channelName;
		std::string _channelTopic;
		std::vector<User*>	_channelMembers;
		std::vector<User*>	_channelOperators;
		std::vector<User*>	_bannedUsers;
		std::vector<User*>	_kickedUsers;
		std::string			_channelPassword;
		size_t				_channelCapacity;
		std::string			_channelSymbol;

};

#endif