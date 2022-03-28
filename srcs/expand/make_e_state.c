/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_e_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:30 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/28 21:51:46 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_expand_state(t_expand_state *e_state)
{
	e_state->start = 0;
	e_state->current_pos = 0;
	e_state->quote_state = NORMAL;
}

int	not_expand_env(t_expand_state *e_state)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (FAIL);
	token->content = ft_strdup(e_state->original_token->content);
	token->attr = e_state->original_token->attr;
	if (ft_lstadd_token(&(e_state->token_list), token) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

t_expand_state	*update_e_state(t_expand_state *e_state, t_list *token_list,
	t_envs *envs, int exit_status)
{
	char	*empty;
	t_token	*new_token;

	while (token_list != NULL)
	{
		e_state->original_token = (t_token *)token_list->content;
		if (ft_strchr(e_state->original_token->content, '$') != NULL)
		{
			empty = ft_strdup("");
			new_token = make_token(empty, 0, 0, TK_WORD);
			if (ft_lstadd_token(&(e_state->token_list), new_token) == FAIL)
				return (NULL);
			free(empty);
			if (expand_env_vals(e_state, envs, exit_status) == FAIL)
				return (NULL);
		}
		else
		{
			if (not_expand_env(e_state) == FAIL)
				return (NULL);
		}
		token_list = token_list->next;
	}
	return (e_state);
}

t_expand_state	*make_e_state(t_list *token_list, t_envs *envs, int exit_status)
{
	t_expand_state	*e_state;

	e_state = (t_expand_state *)malloc(sizeof(t_expand_state));
	if (!e_state)
		return (NULL);
	init_expand_state(e_state);
	e_state->token_list = NULL;
	e_state = update_e_state(e_state, token_list, envs, exit_status);
	if (!e_state)
		return (NULL);
	return (e_state);
}
