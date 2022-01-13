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

t_token_kind	get_attr_about_quote(t_quote_state *state, char c)
{
	t_token_kind	token_kind;

	if (c == '\'' && *state == NORMAL)
		token_kind = TK_SINGLE_QUOTED;
	else if (c == '\"' && *state == NORMAL)
		token_kind = TK_DOUBLE_QUOTED;
	else
		token_kind = TK_WORD;
	return (token_kind);
}

t_token_kind	decide_attr(char *line, int pos, size_t *i, t_quote_state *q_state)
{
	t_token_kind	token_kind;

	(void)q_state;
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

size_t	store_token(char *line, t_list **token_list, size_t *trim_start, size_t current_pos, t_quote_state *q_state)
{
	size_t			i;
	size_t			len;
	size_t			start;
	t_token			*new_token;
	t_token_kind	attr;

	i = 0;
	if ((line[current_pos] == ' ' || line[current_pos] == '\t' || line[current_pos] == '\n') && *q_state == NORMAL)
	{
		attr = decide_attr(line, current_pos, &i, q_state);
		// printf("[separate1]trim_start: %ld current_pos: %ld i: %ld q_state: %d\n", *trim_start, current_pos, i, *q_state);
		if (*trim_start != current_pos)
		{
			new_token = make_token(line, *trim_start, current_pos - *trim_start, attr);
			if (ft_lstadd_node(token_list, new_token) == FAIL)
				return (FAIL);
		}
		current_pos += i;
		*trim_start = current_pos;
	}
	else if ((is_separate_word(line, current_pos) && *q_state == NORMAL))
	{
		attr = decide_attr(line, current_pos, &i, q_state);
		printf("[separate2]trim_start: %ld current_pos: %ld i: %ld q_state: %d\n", *trim_start, current_pos, i, *q_state);
		if (*trim_start != current_pos)
		{
			new_token = make_token(line, *trim_start, current_pos - *trim_start, get_attr_about_quote(q_state, line[current_pos - 1]));
			if (ft_lstadd_node(token_list, new_token) == FAIL)
				return (FAIL);
			start = current_pos;
			len = i;
		}
		else
		{
			start = *trim_start;
			len = current_pos + i - *trim_start;
		}
		new_token = make_token(line, start, len, attr);
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
		printf("seprate2_finish\n");
		current_pos += i;
		*trim_start = current_pos;
	}
	else if (line[current_pos + 1] == '\0')
	{
		attr = get_attr_about_quote(q_state, line[current_pos]);
		current_pos++;
		// printf("[separate3]trim_start: %ld current_pos: %ld i: %ld q_state: %d\n", *trim_start, current_pos, i, *q_state);
		new_token = make_token(line, *trim_start, current_pos - *trim_start, attr);
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
		*trim_start = current_pos + i;
	}
	else
		current_pos++;
	return (current_pos);
}

void	tokenize(char *line, t_list **token_list)
{
	size_t	trim_start;
	size_t	current_pos;
	t_quote_state	q_state;

	q_state = NORMAL;
	trim_start = 0;
	current_pos = 0;
	while (line[current_pos] && current_pos < ft_strlen(line))
	{
		check_quote_state(&q_state, line[current_pos]);
		current_pos = store_token(line, token_list, &trim_start, current_pos, &q_state);
	}
}
