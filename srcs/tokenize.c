#include "minishell.h"

void	check_quote_state(t_quote_state *state, char c)
{
	if (c == '\'' && *state == NORMAL)
		*state = IN_QUOTE;
	else if (c == '\"' && *state == NORMAL)
		*state = IN_DQUOTE;
	else if ((c == '\'' && *state == IN_QUOTE) || (c == '\"' && *state == IN_DQUOTE))
		*state = NORMAL;
}

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

t_token_kind	get_token_kind(char *line, int pos, size_t *i)
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
		while (line[pos + *i] && (line[pos + *i] == ' ' || line[pos + *i] == '\t' || line[pos + *i] == '\n'))
			*i += 1;
		if (pos - 1 > 0 && line[pos - 1] == '\'')
			token_kind = TK_SINGLE_QUOTED;
		else if (pos - 1 > 0 && line[pos - 1] == '\"')
			token_kind = TK_DOUBLE_QUOTED;
		else
			token_kind = TK_WORD;
	}
	return (token_kind);
}

int	is_separate_word(char *line, int pos)
{
	if (is_metacharacter(line[pos]))
		return (1);
	else if ((ft_isdigit(line[pos]) && (line[pos + 1] == '<' || line[pos + 1] == '>')))
		return (1);
	return (0);
}

int	separate_by_no_kind_sep_word(char *line, t_list **token_list, t_tokenizer *tokenizer)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenizer->token_kind = get_token_kind(line, tokenizer->current_pos, &i);
	if (tokenizer->trim_start != tokenizer->current_pos)
	{
		new_token = make_token(line, tokenizer->trim_start, tokenizer->current_pos - tokenizer->trim_start, tokenizer->token_kind);
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
	}
	tokenizer->current_pos += i;
	tokenizer->trim_start = tokenizer->current_pos;
	return (SUCCESS);
}

int	separate_by_sep_word(char *line, t_list **token_list, t_tokenizer *tokenizer)
{
	size_t	i;
	size_t	len;
	size_t	start;
	t_token	*new_token;

	i = 0;
	tokenizer->token_kind = get_token_kind(line, tokenizer->current_pos, &i);
	if (tokenizer->trim_start != tokenizer->current_pos)
	{
		new_token = make_token(line, tokenizer->trim_start, tokenizer->current_pos - tokenizer->trim_start, get_token_kind_about_quote(tokenizer->quote_state, line[tokenizer->current_pos - 1]));
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
		start = tokenizer->current_pos;
		len = i;
	}
	else
	{
		start = tokenizer->trim_start;
		len = tokenizer->current_pos + i - tokenizer->trim_start;
	}
	new_token = make_token(line, start, len, tokenizer->token_kind);
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	tokenizer->current_pos += i;
	tokenizer->trim_start = tokenizer->current_pos;
	return (SUCCESS);
}

int	separate_by_null_char(char *line, t_list **token_list, t_tokenizer *tokenizer)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenizer->current_pos++;
	new_token = make_token(line, tokenizer->trim_start, tokenizer->current_pos - tokenizer->trim_start, get_token_kind_about_quote(tokenizer->quote_state, line[tokenizer->current_pos - 1]));
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	tokenizer->trim_start = tokenizer->current_pos + i;
	return (SUCCESS);
}

int	read_line(char *line, t_list **token_list, t_tokenizer *tokenizer)
{
	if ((line[tokenizer->current_pos] == ' ' || line[tokenizer->current_pos] == '\t' || line[tokenizer->current_pos] == '\n') && tokenizer->quote_state == NORMAL)
		return (separate_by_no_kind_sep_word(line, token_list, tokenizer));
	else if ((is_separate_word(line, tokenizer->current_pos) && tokenizer->quote_state == NORMAL))
		return (separate_by_sep_word(line, token_list, tokenizer));
	else if (line[tokenizer->current_pos + 1] == '\0')
		return (separate_by_null_char(line, token_list, tokenizer));
	else
		tokenizer->current_pos++;
	return (SUCCESS);
}

int	tokenize(char *line, t_list **token_list)
{
	t_tokenizer	tokenizer;

	tokenizer.trim_start = 0;
	tokenizer.current_pos = 0;
	tokenizer.quote_state = NORMAL;
	tokenizer.token_kind = TK_WORD;
	while (line[tokenizer.current_pos] && tokenizer.current_pos < ft_strlen(line))
	{
		check_quote_state(&tokenizer.quote_state, line[tokenizer.current_pos]);
		if (read_line(line, token_list, &tokenizer) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
