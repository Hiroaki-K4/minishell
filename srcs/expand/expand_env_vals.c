#include "minishell.h"

char	*get_env_name(t_expand_state *e_state, size_t start)
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
			return (ft_strdup("$?"));
	}
	if (e_state->current_pos == start)
		return (ft_strdup("$"));
	return (ft_substr(e_state->original_token->content,
			start, e_state->current_pos - start));
}

t_expand_state	*fill_diff_between_start_curernt_pos(t_expand_state *e_state)
{
	char	*add_word;

	if (e_state->start != e_state->current_pos)
	{
		add_word = ft_substr(e_state->original_token->content, e_state->start,
				e_state->current_pos - e_state->start);
		if (!add_word)
			return (NULL);
		if (ft_lstadd_word(&(e_state->token_list), add_word) == FAIL)
			return (NULL);
		free(add_word);
	}
	return (e_state);
}

int	get_env_value_and_insert(char *name, t_envs *envs, t_expand_state *e_state)
{
	char	*value;
	t_list	*tmp_list;

	value = get_env(name, envs);
	if (value != NULL)
	{
		if (e_state->quote_state == NORMAL)
		{
			tmp_list = NULL;
			if (tokenize(value, &tmp_list) == FAIL)
				return (FAIL);
			if (ft_lstadd_last(&(e_state->token_list), tmp_list) == FAIL)
				return (FAIL);
		}
		else
		{
			if (ft_lstadd_word(&(e_state->token_list), value) == FAIL)
				return (FAIL);
		}
	}
	free(value);
	return (SUCCESS);
}

int	check_name(char *name, t_envs *envs, t_expand_state *e_state,
	int exit_status)
{
	char	*status;

	if (!ft_strncmp(name, "$", ft_strlen(name) + 1))
	{
		if (ft_lstadd_word(&(e_state->token_list), name) == FAIL)
			return (FAIL);
	}
	else if (!ft_strncmp(name, "$?", ft_strlen(name) + 1))
	{
		status = ft_itoa(exit_status);
		if (!status)
			return (FAIL);
		if (ft_lstadd_word(&(e_state->token_list), status) == FAIL)
			return (FAIL);
		free(status);
	}
	else
	{
		if (get_env_value_and_insert(name, envs, e_state) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

int	expand_env_vals(t_expand_state *e_state, t_envs *envs, int exit_status)
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
			e_state = fill_diff_between_start_curernt_pos(e_state);
			if (!e_state)
				return (FAIL);
			e_state->current_pos++;
			name = get_env_name(e_state, e_state->current_pos);
			if (!name)
				return (FAIL);
			if (check_name(name, envs, e_state, exit_status) == FAIL)
				return (FAIL);
			free(name);
			e_state->start = e_state->current_pos;
			continue ;
		}
		e_state->current_pos++;
	}
	e_state = fill_diff_between_start_curernt_pos(e_state);
	if (!e_state)
		return (FAIL);
	return (SUCCESS);
}
