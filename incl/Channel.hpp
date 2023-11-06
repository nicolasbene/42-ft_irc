/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:10:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/06 18:36:52 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
# include <iostream>
# include <Server.hpp>
# include <User.hpp>

class Channel
{
	public:
		Channel(std::string channelName);
		~Channel();

	private:
		std::string _channelName;
		std::string _channelTopic;
		std::vector<User> _channelMembers;
		int _usercount;
		int _userlimit;
};

#endif