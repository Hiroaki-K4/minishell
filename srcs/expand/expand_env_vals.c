#include "minishell.h"

char	*get_name_after_dollar(t_expand_state *e_state, size_t start)
{
	while (e_state->original_token->content[e_state->current_pos]
		&& e_state->original_token->content[e_state->current_pos] != ' '
		&& e_state->original_token->content[e_state->current_pos] != '$'
		&& e_state->original_token->content[e_state->current_pos] != '\''
		&& e_state->original_token->content[e_state->current_pos] != '\"'
	)
	{
		e_state->current_pos++;
		if (e_state->original_token->content[e_state->current_pos - 1] == '?')
		{
			// TODO: Correspond $?
			printf("$? appeared\n");
			continue ;
		}
	}
	if (e_state->current_pos == start)
		return (ft_strdup("$"));
	return (ft_substr(e_state->original_token->content,
			start, e_state->current_pos - start));
}

void	check_diff_between_start_and_curernt_pos(t_expand_state *e_state)
{
	if (e_state->start != e_state->current_pos)
		ft_lstadd_word(&(e_state->token_list),
			ft_substr(e_state->original_token->content, e_state->start,
				e_state->current_pos - e_state->start));
}

void	get_value_and_insert(char *name, t_envs *envs, t_expand_state *e_state)
{
	char	*value;
	t_list	*tmp_list;

	value = get_env(name, envs);
	if (value != NULL)
	{
		if (e_state->quote_state == NORMAL)
		{
			tmp_list = NULL;
			tokenize(value, &tmp_list);
			ft_lstadd_last(&(e_state->token_list), tmp_list);
		}
		else
			ft_lstadd_word(&(e_state->token_list), value);
	}
}

int	check_name(char *name, t_envs *envs, t_expand_state *e_state)
{
	if (ft_strncmp(name, "$", ft_strlen(name) + 1) == 0)
	{
		if (ft_lstadd_word(&(e_state->token_list), name) == FAIL)
			return (FAIL);
	}
	else
		get_value_and_insert(name, envs, e_state);
	return (SUCCESS);
}

int	expand_env_vals(t_expand_state *e_state, t_envs *envs)
{
	char	*name;

	init_expand_state(e_state);
	while (e_state->original_token->content[e_state->current_pos])
	{
		update_quote_state(&(e_state->quote_state),
			e_state->original_token->content[e_state->current_pos]);
		if (e_state->original_token->content[e_state->current_pos] == '$'
			&& e_state->quote_state != IN_QUOTE)
		{
			check_diff_between_start_and_curernt_pos(e_state);
			e_state->current_pos++;
			name = get_name_after_dollar(e_state, e_state->current_pos);
			if (check_name(name, envs, e_state) == FAIL)
				return (FAIL);
			e_state->start = e_state->current_pos;
			continue ;
		}
		e_state->current_pos++;
	}
	check_diff_between_start_and_curernt_pos(e_state);
	return (SUCCESS);
}
