/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 21:08:55 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/25 22:46:05 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

https://github.com/osslate/irc-message/blob/master/index.js

# include "constant.h"
# include "struct.h"
# include "error.h"

t_parse_return parse_tags(std::string tagStr, t_message *messageParsed)
{
	tagStr.copy(messageParsed->tags, tagStr.length());
	return (PARSING_SUCCESS);
}

t_parse_return parse_prefix(std::string tagStr, t_message *messageParsed)
{
	return (PARSING_SUCCESS);
}

t_parse_return parse_cmd(std::string tagStr, t_message *messageParsed)
{
	return (PARSING_SUCCESS);
}

t_parse_return parse_parameters(std::string tagStr, t_message *messageParsed)
{
	return (PARSING_SUCCESS);
}

t_parse_return parser(std::string strToParse, t_message *messageParsed)
{
	if (strToParse.empty())
		return (PARSING_EMPTY_MESSAGE);
	std::string::iterator position;
	std::string::iterator nextSpace;
	size_t spacePos = 0;
	position= strToParse.begin();

	// Check if tags are presents
	if (*position == '@') {
		spacePos = strToParse.find(' ', spacePos);
		std::string tagStr;

		if (spacePos != std::string::npos)
			tagStr = std::string(position, position + spacePos);
		else 
			return (PARSING_GRAMMAR_ERROR);
		t_parse_return ret = parse_tags(tagStr, messageParsed);
		if (ret != 0)
			return (ret);
		position += spacePos;
	}

	// Skip spaces
	while (*position == ' ')
		position ++;

	// Check if prefix are presents
	if (*position == ':') {
		spacePos = strToParse.find(' ', spacePos);
		std::string prefixStr;

		if (spacePos != std::string::npos)
			prefixStr = std::string(position, position + spacePos);
		else 
			return (PARSING_GRAMMAR_ERROR);
		t_parse_return ret = parse_prefix(prefixStr, messageParsed);
		if (ret != 0)
			return (ret);
		position += spacePos;
	}

	// Skip spaces
	while (*position == ' ')
		position ++;

	// Check if there is nothing after the command
	spacePos = strToParse.find(' ', spacePos);

	if (spacePos == std::string::npos) {
		std::string cmdStr = std::string(position, strToParse.end());
		return (parse_cmd(cmdStr, messageParsed));
	}

	// If something after cmd, we expect some parameters
	// But first we parse the cmd
	std::string cmdStr = std::string(position, position + spacePos);

	t_parse_return ret = parse_cmd(cmdStr, messageParsed);

	if (ret != 0)
		return (ret);

	// Skip spaces
	while (*position == ' ')
		position ++;

	spacePos = strToParse.find(' ', spacePos);

	// Parse all parameter except the last one
	while (spacePos != std::string::npos) {
		std::string paramStr = std::string(position, position + spacePos);
		t_parse_return ret = parse_parameters(paramStr, messageParsed);
		if (ret != 0)
			return (ret);
		position += spacePos;
		while (*position == ' ')
			position ++;
		spacePos = strToParse.find(' ', spacePos);	
	}

	// Last parameter
	std::string paramStr = std::string(position, strToParse.end());
	ret = parse_parameters(paramStr, messageParsed);
	if (ret != 0)
		return (ret);

	return (PARSING_SUCCESS);
}