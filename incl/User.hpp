/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:46:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/08 12:58:28 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

# include <string>
# include <iostream>
//# include "Channel.hpp"

class User
{
	public:

		User(int sockId, const std::string& userName);
		~User();
		bool operator!=(User& other) {return (this->_userSockId != other.getUserSockId());}

		void setUserName(const std::string userName);
		void setUserNickName(const std::string userNickName);
		int getUserSockId(void);
		std::string getUserName(void);
		std::string getUserNickName(void);
		void addChannelList(const std::string userNickName);

	private:
	int	_userSockId;
	std::string _userName;
	std::string _nickName;
	bool	_isConnected;
	//std::vector<Channel> _channelList;

};

#endif