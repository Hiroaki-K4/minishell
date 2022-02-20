/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:26:20 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:26:21 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_env_pos(char *env_name, t_envs *envs)
{
	int		ret;
	size_t	i;
	size_t	j;
	char	*name;

	i = 0;
	ret = -1;
	while (envs->content[i])
	{
		j = 0;
		while (envs->content[i][j] && envs->content[i][j] != '=')
			j++;
		name = ft_substr(envs->content[i], 0, j);
		if (!ft_strncmp(env_name, name, ft_strlen(env_name) + 1))
		{
			free(name);
			ret = i;
			break ;
		}
		free(name);
		i++;
	}
	return (ret);
}

int	make_new_envs(char *name, char *val, t_envs **envs)
{
	char	**new_envs;
	char	*tmp;
	size_t	i;

	new_envs = (char **)malloc(sizeof(char *) * ((*envs)->envs_num + 2));
	if (!new_envs)
		return (FAIL);
	i = -1;
	while ((*envs)->content[++i])
		new_envs[i] = (*envs)->content[i];
	if (val == NULL)
		new_envs[i] = ft_strdup(name);
	else
	{
		tmp = ft_strjoin(name, "=");
		new_envs[i] = ft_strjoin(tmp, val);
		free(tmp);
	}
	new_envs[i + 1] = NULL;
	free((*envs)->content);
	(*envs)->content = new_envs;
	(*envs)->envs_num++;
	return (SUCCESS);
}

int	set_env(char *name, char *val, t_envs **envs)
{
	int		env_pos;
	char	*tmp;

	env_pos = get_env_pos(name, *envs);
	if (env_pos != -1)
	{
		if (val != NULL)
		{
			free((*envs)->content[env_pos]);
			tmp = ft_strjoin(name, "=");
			(*envs)->content[env_pos] = ft_strjoin(tmp, val);
			free(tmp);
		}
	}
	else
	{
		if (make_new_envs(name, val, envs) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
