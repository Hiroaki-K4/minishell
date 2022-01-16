#include "minishell.h"

int	separate_by_no_kind_sep_word(char *line, t_list **token_list,
	t_tokenize_state *tokenize_state)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenize_state->token_kind = get_token_kind(line,
			tokenize_state->current_pos, &i);
	if (tokenize_state->trim_start != tokenize_state->current_pos)
	{
		new_token = make_token(line, tokenize_state->trim_start,
				tokenize_state->current_pos - tokenize_state->trim_start,
				tokenize_state->token_kind);
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
	}
	tokenize_state->current_pos += i;
	tokenize_state->trim_start = tokenize_state->current_pos;
	return (SUCCESS);
}

int	separate_by_sep_word(char *line, t_list **token_list,
	t_tokenize_state *tokenize_state)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenize_state->token_kind = get_token_kind(line,
			tokenize_state->current_pos, &i);
	if (tokenize_state->trim_start != tokenize_state->current_pos)
	{
		new_token = make_token(line, tokenize_state->trim_start,
				tokenize_state->current_pos - tokenize_state->trim_start,
				get_token_kind_about_quote(tokenize_state->quote_state,
					line[tokenize_state->current_pos - 1]));
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
		tokenize_state->trim_start = tokenize_state->current_pos;
	}
	new_token = make_token(line, tokenize_state->trim_start,
			tokenize_state->current_pos + i - tokenize_state->trim_start,
			tokenize_state->token_kind);
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	tokenize_state->current_pos += i;
	tokenize_state->trim_start = tokenize_state->current_pos;
	return (SUCCESS);
}

int	separate_by_null_char(char *line, t_list **token_list,
	t_tokenize_state *tokenize_state)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenize_state->current_pos++;
	new_token = make_token(line, tokenize_state->trim_start,
			tokenize_state->current_pos - tokenize_state->trim_start,
			get_token_kind_about_quote(tokenize_state->quote_state,
				line[tokenize_state->current_pos - 1]));
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	tokenize_state->trim_start = tokenize_state->current_pos + i;
	return (SUCCESS);
}
