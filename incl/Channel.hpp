/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:10:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/17 18:56:43 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
# include <iostream>
# include "Server.hpp"
# include "User.hpp"
# include "Log.hpp"

class User;
class Server;

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
		const std::string getName() const;
		const std::string getPassword() const;
		size_t getChannelCap() const;
		const std::string getSymbol() const;

		void	setPassword(std::string pass);
		void	setChannelCap(int cap);

		std::string listOfMember() const;

		//Mode
		// Ajout de nouvelles fonctions nécessaires
		void addOperator(User& targetUserObject, const std::string& targetUser);
		
        // bool isOperator(const std::string& userName) const;
		
		// void addKey(const std::string& key);
		// void removeKey(const std::string& key);
		// void addLimit(const std::string& limit);
		// void removeLimit(const std::string& limit);
		// void addTopic(const std::string& topic);
		// void removeTopic(const std::string& topic);
		// void addInviteOnly(const std::string& user);
		// void removeInviteOnly(const std::string& user);
		


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