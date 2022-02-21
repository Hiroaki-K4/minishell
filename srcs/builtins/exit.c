/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:02 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:47:04 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **argv, t_envs **envs)
{
	(void)envs;
	ft_putendl_fd("exit", 1);
	if (argv[1] == NULL)
		exit(EXIT_SUCCESS);
	else if (argv[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 1);
		return (EXIT_FAILURE);
	}
	exit(ft_atoi(argv[1]));
}
