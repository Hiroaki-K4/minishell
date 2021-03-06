/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:26:25 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:26:26 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_env(int index, t_envs **envs)
{
	size_t	i;
	size_t	pos;
	char	**tmp;

	tmp = (char **)malloc(sizeof(char *) * ((*envs)->envs_num));
	if (!tmp)
		return (FAIL);
	i = 0;
	pos = 0;
	while ((*envs)->content[i])
	{
		if (i != (size_t)index)
		{
			tmp[pos] = ft_strdup((*envs)->content[i]);
			pos++;
		}
		i++;
	}
	tmp[pos] = NULL;
	free_strings((*envs)->content);
	(*envs)->content = tmp;
	(*envs)->envs_num--;
	return (SUCCESS);
}

int	ft_unset(char **argv, t_envs **envs)
{
	int		index;
	size_t	i;

	if (argv[1] != NULL)
	{
		i = 1;
		while (argv[i])
		{
			index = get_env_pos(argv[i], *envs);
			if (index != -1)
			{
				if (remove_env(index, envs) == FAIL)
					return (EXIT_FAILURE);
			}
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
