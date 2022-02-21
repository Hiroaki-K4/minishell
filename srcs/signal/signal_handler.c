/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:52 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:48:53 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_sigaction(struct sigaction *sigaction, void (*handler)(int))
{
	ft_bzero(sigaction, sizeof(*sigaction));
	sigaction->sa_handler = handler;
}

void	sigint_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler2(int sig)
{
	(void)sig;
	write(1, "\b\b  \n", 5);
}
