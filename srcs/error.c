/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:49:01 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:49:02 by hkubo            ###   ########.fr       */
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
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	if (msg == NULL)
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putendl_fd(msg, 2);
}
