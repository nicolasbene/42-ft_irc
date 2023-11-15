/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 22:52:47 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/15 14:39:34 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class Message {
private:
	std::string rawMessage;
	std::string prefixe;
	std::string commande;
	std::vector<std::string> parameters;
	std::string	trailing;

public:
	Message(const std::string& raw);
	~Message();

	std::string getPrefixe() const {
		return prefixe;
	}

	std::string getRawMessage() const {
		return rawMessage;
	}

	std::string getCommande() const {
		return commande;
	}

	std::string getTrailing() const {
		return trailing;
	}

	std::vector<std::string> getParameters() const {
		return parameters;
	}
};

#endif