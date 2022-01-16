#include "minishell.h"

t_token_kind	get_token_kind_about_quote(t_quote_state state, char c)
{
	t_token_kind	token_kind;

	if (c == '\'' && state == NORMAL)
		token_kind = TK_SINGLE_QUOTED;
	else if (c == '\"' && state == NORMAL)
		token_kind = TK_DOUBLE_QUOTED;
	else
		token_kind = TK_WORD;
	return (token_kind);
}

t_token_kind	check_whether_quoted_word(char *line, int pos, size_t *i)
{
	t_token_kind	token_kind;

	while (line[pos + *i] && (line[pos + *i] == ' '
			|| line[pos + *i] == '\t' || line[pos + *i] == '\n'))
		*i += 1;
	if (pos - 1 > 0 && line[pos - 1] == '\'')
		token_kind = TK_SINGLE_QUOTED;
	else if (pos - 1 > 0 && line[pos - 1] == '\"')
		token_kind = TK_DOUBLE_QUOTED;
	else
		token_kind = TK_WORD;
	return (token_kind);
}

t_token_kind	get_token_kind(char *line, int pos, size_t *i)
{
	t_token_kind	token_kind;

	token_kind = TK_WORD;
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
		token_kind = check_whether_quoted_word(line, pos, i);
	return (token_kind);
}
