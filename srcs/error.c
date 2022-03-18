/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:49:01 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/18 00:07:26 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_error(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	perror(msg);
	exit(errno);
}

void	print_command_error(char *command, char *msg)
{
	if (command == NULL)
		return ;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	if (msg == NULL)
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putendl_fd(msg, 2);
}

void	print_error(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(msg, 2);
}
