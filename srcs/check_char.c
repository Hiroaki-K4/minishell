#include "minishell.h"

int	is_metacharacter_with_token_kind(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_metacharacter_without_token_kind(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_separating_word(char *line, int pos)
{
	if (is_metacharacter_with_token_kind(line[pos]))
		return (1);
	else if ((ft_isdigit(line[pos]) && (line[pos + 1] == '<'
				|| line[pos + 1] == '>')))
		return (1);
	return (0);
}
