#include "minishell.h"

int	separate_by_no_kind_sep_word(char *line, t_list **token_list,
	t_tokenize_state *t_state)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	t_state->token_kind = get_token_kind(line,
			t_state->current_pos, &i);
	if (t_state->trim_start != t_state->current_pos)
	{
		new_token = make_token(line, t_state->trim_start,
				t_state->current_pos - t_state->trim_start,
				t_state->token_kind);
		if (!new_token)
			return (FAIL);
		if (ft_lstadd_token(token_list, new_token) == FAIL)
			return (FAIL);
	}
	t_state->current_pos += i;
	t_state->trim_start = t_state->current_pos;
	return (SUCCESS);
}

int	separate_by_sep_word(char *line, t_list **token_list,
	t_tokenize_state *t_state)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	t_state->token_kind = get_token_kind(line,
			t_state->current_pos, &i);
	if (t_state->trim_start != t_state->current_pos)
	{
		new_token = make_token(line, t_state->trim_start,
				t_state->current_pos - t_state->trim_start,
				TK_WORD);
		if (!new_token)
			return (FAIL);
		if (ft_lstadd_token(token_list, new_token) == FAIL)
			return (FAIL);
		t_state->trim_start = t_state->current_pos;
	}
	new_token = make_token(line, t_state->trim_start,
			t_state->current_pos + i - t_state->trim_start,
			t_state->token_kind);
	if (!new_token)
		return (FAIL);
	if (ft_lstadd_token(token_list, new_token) == FAIL)
		return (FAIL);
	t_state->current_pos += i;
	t_state->trim_start = t_state->current_pos;
	return (SUCCESS);
}

int	separate_by_null_char(char *line, t_list **token_list,
	t_tokenize_state *t_state)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	t_state->current_pos++;
	new_token = make_token(line, t_state->trim_start,
			t_state->current_pos - t_state->trim_start, TK_WORD);
	if (!new_token)
		return (FAIL);
	if (ft_lstadd_token(token_list, new_token) == FAIL)
		return (FAIL);
	t_state->trim_start = t_state->current_pos + i;
	return (SUCCESS);
}
