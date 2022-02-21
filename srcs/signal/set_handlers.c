/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:47 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:48:49 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_initial_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, sigint_handler);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_child_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, SIG_DFL);
	set_sigaction(&state->sa_sigquit, SIG_DFL);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_parent_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, SIG_IGN);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_redirect_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, sigint_handler2);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}
