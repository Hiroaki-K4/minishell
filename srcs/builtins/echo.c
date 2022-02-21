/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:46:12 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:46:13 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **argv, t_envs *envs)
{
	int		has_n_option;
	size_t	i;

	(void)envs;
	if (argv[1] != NULL && !ft_strncmp(argv[1], "-n", 3))
	{
		has_n_option = TRUE;
		i = 2;
	}
	else
	{
		has_n_option = FALSE;
		i = 1;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
		if (argv[i] != NULL)
			write(1, " ", 1);
	}
	if (!has_n_option)
		write(1, "\n", 1);
	return (SUCCESS);
}
