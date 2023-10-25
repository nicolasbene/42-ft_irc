#ifndef CONSTANT_H
# define CONSTANT_H

typedef enum e_cmd_type
{
	PASS = 0,
	NICK,
	USER,
	JOIN,
	PART,
	LEAVE,
	PRIVMSG,
	QUIT,
	KICK,
	INVITE,
	TOPIC,
	MODE
}	t_cmd_type;

typedef enum e_parse_return
{
	PARSING_SUCCESS = 0,
	PARSING_EMPTY_MESSAGE,
	PARSING_GRAMMAR_ERROR
}	t_parse_return;

#endif
