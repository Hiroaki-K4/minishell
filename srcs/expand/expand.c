/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:26 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:25:27 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs,
	int exit_status)
{
	t_expand_state	*e_state;

	e_state = make_e_state(token_list, envs, exit_status);
	if (!e_state)
		return (FAIL);
	*expanded_list = check_quote(e_state);
	if (!*expanded_list)
		return (FAIL);
	free(e_state);
	return (SUCCESS);
}
