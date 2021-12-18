#include "minishell.h"

t_token_kind	check_quote(char *line, size_t pos, size_t *i)
{
	size_t	idx;
	char	quote_start;

	idx = 1;
	quote_start = line[pos];
	while (line[pos + idx] != quote_start && line[pos + idx])
		idx++;
	*i += idx;
	if (line[pos + idx] == quote_start && quote_start == '\'')
		return (TK_SINGLE_QUOTED);
	else if (line[pos + idx] == quote_start && quote_start == '\"')
		return (TK_DOUBLE_QUOTED);
	else
	{
		printf("invalid syntax: %s, %zu\n", line, pos);
		exit(EXIT_FAILURE);
	}
}

t_token_kind	decide_attr(char *line, int pos, size_t *i)
{
	t_token_kind	token_kind;

	if (line[pos] == '|')
	{
		*i += 1;
		token_kind = TK_PIPE;
	}
	else if ((ft_isdigit(line[pos]) && (line[pos + 1] == '<' || line[pos + 1] == '>')))
	{
		*i += 1;
		token_kind = TK_IO_NUMBER;
	}
	else if ((line[pos] == '<' && line[pos + 1] == '<'))
	{
		*i += 2;
		token_kind = TK_REDIRECT_MULTI;
	}
	else if (line[pos] == '>' && line[pos + 1] == '>')
	{
		*i += 2;
		token_kind = TK_REDIRECT_APPEND;
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
	else if (line[pos] == '\"' || line[pos] == '\'')
	{
		*i += 1;
		token_kind = check_quote(line, pos, i);
	}
	else
	{
		while (!is_metacharacter(line[pos + *i]))
			*i += 1;
		token_kind = TK_WORD;
	}
	return (token_kind);
}

size_t	store_token(char *line, t_list **token_list, size_t pos)
{
	size_t			i;
	t_token			*new_token;
	t_token_kind	attr;

	i = 0;
	attr = decide_attr(line, pos, &i);
	new_token = make_token(line, pos, i, attr);
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	return (pos + i);
}

void	tokenize(char *line, t_list **token_list)
{
	size_t	pos;

	pos = 0;
	while (line[pos] && pos < ft_strlen(line))
	{
		if (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\n')
		{
			pos++;
			continue ;
		}
		pos = store_token(line, token_list, pos);
	}
}
