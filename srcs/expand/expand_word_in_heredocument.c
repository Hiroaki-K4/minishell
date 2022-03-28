/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_in_heredocument.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 17:00:30 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/28 21:26:02 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_name_here_ver(char *name, t_envs *envs, int exit_status)
{
	char	*value;

	if (!ft_strncmp(name, "$", ft_strlen(name) + 1))
		return (ft_strdup(name));
	else if (!ft_strncmp(name, "$?", ft_strlen(name) + 1))
		return (ft_itoa(exit_status));
	else
	{
		value = get_env(name, envs);
		return (value);
	}
}

char	*expand_core_here_ver(char *input, size_t *current_pos,
	t_envs *envs, int exit_status)
{
	char	*name;
	char	*expanded;

	(*current_pos)++;
	name = get_env_name_here_ver(input, current_pos);
	if (!name)
		return (NULL);
	expanded = check_name_here_ver(name, envs, exit_status);
	free(name);
	return (expanded);
}

char	*fill_diff_here_ver(char *input,
	char *result, size_t start, size_t current_pos)
{
	char	*add_word;
	char	*tmp;

	add_word = ft_substr(input, start, current_pos - start);
	tmp = ft_strjoin(result, add_word);
	free(result);
	free(add_word);
	result = tmp;
	return (result);
}

char	*expand_here_ver(char *input, t_envs *envs, int status, size_t start)
{
	size_t	c_pos;
	char	*tmp;
	char	*result;
	char	*add_word;

	result = ft_strdup("");
	c_pos = 0;
	while (input[c_pos])
	{
		if (input[c_pos] == '$')
		{
			if (start != c_pos)
				result = fill_diff_here_ver(input, result, start, c_pos);
			add_word = expand_core_here_ver(input, &c_pos, envs, status);
			if (add_word == NULL)
				add_word = ft_strdup("");
			start = c_pos;
			tmp = ft_strjoin(result, add_word);
			free_double_word(result, add_word);
			result = tmp;
			continue ;
		}
		c_pos++;
	}
	if (start != c_pos)
		result = fill_diff_here_ver(input, result, start, c_pos);
	return (result);
}

char	*expand_word_in_heredocument(char *input, t_envs *envs, int exit_status)
{
	char	*expanded;

	if (ft_strchr(input, '$') != NULL)
	{
		expanded = expand_here_ver(input, envs, exit_status, 0);
		return (expanded);
	}
	return (ft_strdup(input));
}
