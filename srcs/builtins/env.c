/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:26:04 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:26:05 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **argv, t_envs *envs)
{
	size_t	i;

	if (argv[1] == NULL)
	{
		i = 0;
		while (envs->content[i])
		{
			if (ft_strchr(envs->content[i], '=') != NULL)
				printf("%s\n", envs->content[i]);
			i++;
		}
	}
	return (SUCCESS);
}
