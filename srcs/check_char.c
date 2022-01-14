#include "minishell.h"

int	is_metacharacter(char c)
{
	return (c == ' ' || c == '\n' || c == '\t'
		|| c == '|' || c == '<' || c == '>'
		|| c == '(' || c == ')' || c == ';');
}

int	is_separating_word(char *line, int pos)
{
	if (is_metacharacter(line[pos]))
		return (1);
	else if ((ft_isdigit(line[pos]) && (line[pos + 1] == '<' || line[pos + 1] == '>')))
		return (1);
	return (0);
}
