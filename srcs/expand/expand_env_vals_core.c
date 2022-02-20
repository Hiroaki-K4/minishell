/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vals_core.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:11 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:25:12 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_expand_state	*expand_env_vals_core(t_expand_state *e_state, t_envs *envs,
		int exit_status)
{
	char	*name;

	e_state = fill_diff_between_start_curernt_pos(e_state);
	if (!e_state)
		return (NULL);
	e_state->current_pos++;
	name = get_env_name(e_state, e_state->current_pos);
	if (!name)
		return (NULL);
	if (check_name(name, envs, e_state, exit_status) == FAIL)
		return (NULL);
	free(name);
	e_state->start = e_state->current_pos;
	return (e_state);
}
