/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:19 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:25:20 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_env_vals(t_expand_state *e_state, t_envs *envs, int exit_status)
{
	init_expand_state(e_state);
	while (e_state->original_token->content[e_state->current_pos])
	{
		update_quote_state(&(e_state->quote_state),
			e_state->original_token->content[e_state->current_pos]);
		if (e_state->original_token->content[e_state->current_pos] == '$'
			&& e_state->quote_state != IN_QUOTE)
		{
			e_state = expand_env_vals_core(e_state, envs, exit_status);
			if (!e_state)
				return (FAIL);
			continue ;
		}
		e_state->current_pos++;
	}
	e_state = fill_diff_between_start_curernt_pos(e_state);
	if (!e_state)
		return (FAIL);
	return (SUCCESS);
}
