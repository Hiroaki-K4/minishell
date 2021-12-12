#include "minishell.h"

int	is_metacharacter(char c)
{
	return (c == ' ' || c == '\n' || c == '\t'
		|| c == '|' || c == '<' || c == '>'
		|| c == '(' || c == ')' || c == ';');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_separating_character(char c)
{
	return (is_metacharacter(c) || is_quotes(c));
}
