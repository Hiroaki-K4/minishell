#include "minishell.h"

t_token_kind	get_token_kind(char *line, int pos, size_t *i)
{
	t_token_kind	token_kind;

	if (line[pos] == '|')
	{
		*i += 1;
		token_kind = TK_PIPE;
	}
	else if ((ft_isdigit(line[pos])
			&& (line[pos + 1] == '<' || line[pos + 1] == '>')))
	{
		*i += 1;
		token_kind = TK_IO_NUMBER;
	}
	else if ((line[pos] == '<' && line[pos + 1] == '<'))
	{
		*i += 2;
		token_kind = TK_REDIRECT_DLESS;
	}
	else if (line[pos] == '>' && line[pos + 1] == '>')
	{
		*i += 2;
		token_kind = TK_REDIRECT_DGREAT;
	}
	else if (line[pos] == '<')
	{
		*i += 1;
		token_kind = TK_REDIRECT_IN;
	}
	else if (line[pos] == '>')
	{
		*i += 1;
		token_kind = TK_REDIRECT_OUT;
	}
	else if (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\n')
	{
		while (line[pos + *i] && (line[pos + *i] == ' '
				|| line[pos + *i] == '\t' || line[pos + *i] == '\n'))
			*i += 1;
		token_kind = TK_WORD;
	}
	else
		token_kind = TK_WORD;
	return (token_kind);
}
